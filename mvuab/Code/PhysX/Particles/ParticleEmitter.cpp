#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "Texture/Texture.h"
#include "EngineManagers.h"
#include "Effects/EffectManager.h"
#include "RenderableVertex/InstancedVertexTypes.h"
#include "RenderableVertex/InstancingVertexs.h"
#include "Shapes/Shapes.h"

#define MAX_PARTICLES 800
using namespace baseUtils;

const uint32 lIdxCount = 6;
const uint32 lVtxCount = 4;

TT1_VERTEX lVtx[lVtxCount] =
{
  {  -0.5f, 0.0f, -0.5f, 0, 0 },    // vertex 0
  {  -0.5f, 0.0f,  0.5f, 0, 1 },    // vertex 1
  {   0.5f, 0.0f,  0.5f, 1, 1 },    // vertex 2
  {   0.5f, 0.0f, -0.5f, 1, 0 }     // vertex 3
};

unsigned short int lIdx[lIdxCount] = { 0, 1, 2,  2, 3, 0 };

#define ALLOC_PARTICLES_PROPERTY( particle_property, particle_property_type, size ) \
      particle_property = new particle_property_type[size];\
      ZeroMemory( particle_property, size * sizeof( particle_property_type ) );\

CParticleEmitter::CParticleEmitter( ps::TEmitterType aType, ps::TSpawnFunction aFunction )
  : CName()
  , mRV( 0 )
  , mType( aType )
  , mSpawnFn( aFunction )
  , mIsLoop( false )
  , mIsActive( false )
  , mAliveParticlesCount( 0 )
  , mTTLEmitter( 0.0f )
  , mTechnique( NULL )
  , mRandomInitialAngle( false )
  , mEmissionTime( 0.0f )
  , mTimeSinceLastEmission( 0.0f )
  , mMaxLife( 0.0f )
  , mActualTime( 0.0f )
  , mUseFixedDirection( false )
{
}

CParticleEmitter::~CParticleEmitter()
{
  CHECKED_DELETE_ARRAY( mParticlesStream );
  CHECKED_DELETE_ARRAY( mParticlesTTL );
  CHECKED_DELETE_ARRAY( mParticlesLifeTime );
  CHECKED_DELETE_ARRAY( mParticlesAngles );
  CHECKED_DELETE_ARRAY( mParticlesSpeed );
  CHECKED_DELETE_ARRAY( mParticlesRadialSpeed );
  CHECKED_DELETE_ARRAY( mParticlesIsAlive );
  CHECKED_DELETE_ARRAY( mParticlesFlipUVHorizontal);
  CHECKED_DELETE_ARRAY( mParticlesFlipUVVertical);
  CHECKED_DELETE_ARRAY( mParticlesDirections );
  CHECKED_DELETE_ARRAY( mParticlesPositions );
  CHECKED_DELETE( mRV );
}

void CParticleEmitter::LoadFromNode( const CXMLTreeNode& atts )
{
  SetName( atts.GetAttribute<std::string>( "name", "no_name" ) );
  mIsLoop               = atts.GetAttribute<bool>( "loop", false );
  mIsActive             = atts.GetAttribute<bool>( "active", false );
  mTTLEmitter           = atts.GetAttribute<float>( "time_to_live", 0.0f );

  for ( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
  {
    const CXMLTreeNode& lSubNode = atts( i );
    const std::string& lTagName = lSubNode.GetName();

    if ( lTagName == "particles" )
    {
      mMaxAliveParticles    = lSubNode.GetAttribute<uint32>( "max_alive_particles", 0 );
      mTTLParticles         = lSubNode.GetAttribute < Math::Vect2f > ( "time_to_live"     , Math::Vect2f() );

      for ( uint32 iParticles = 0, lCount = lSubNode.GetNumChildren(); iParticles < lCount; ++iParticles )
      {
        const CXMLTreeNode& lParticleNode = lSubNode( iParticles );
        const std::string& lTagParticleName = lParticleNode.GetName();

        if ( lTagParticleName == "texture" )
        {
          mTexture.mValue               = lParticleNode.GetAttribute<CTexture>( "name" );
          mTexture.mValue2              = lParticleNode.GetAttribute<CTexture>( "name2" );
          mTexture.mFlipUVHorizontal    = lParticleNode.GetAttribute<bool>( "flip_uv_horizontal", false );
          mTexture.mFlipUVVertical      = lParticleNode.GetAttribute<bool>( "flip_uv_vertical"  , false );
        }
        else if ( lTagParticleName == "motion" )
        {
          mMotion.mGravity              = lParticleNode.GetAttribute<float>( "gravity", 0.0f );
          mMotion.mLinearSpeed          = lParticleNode.GetAttribute < Math::Vect2f > ( "linear_speed"     , Math::Vect2f() );
          mMotion.mRadialSpeed          = lParticleNode.GetAttribute < Math::Vect2f > ( "radial_speed"     , Math::Vect2f() );
          mMotion.mInitialDirectionMin  = lParticleNode.GetAttribute < Math::Vect3f > ( "initial_dir_min"  , Math::Vect3f() );
          mMotion.mInitialDirectionMax  = lParticleNode.GetAttribute < Math::Vect3f > ( "initial_dir_max"  , Math::Vect3f() );
        }
        else if ( lTagParticleName == "technique" )
        {
          mTechnique = lParticleNode.GetAttribute<CEffectTechnique>( "name" );
        }
        else if ( lTagParticleName == "interpolators" )
        {
          mInterpolator = CParticleInterpolator( lParticleNode );
        }
      }
    }
    else if ( lTagName == "emissions" )
    {
      mParticlesPerEmission = lSubNode.GetAttribute < Math::Vect2u > ( "particles_per_emission"     , Math::Vect2u() );
      mRandomInitialAngle = lSubNode.GetAttribute<bool>( "random_initial_angle"  , false );
      mEmissionTime       = lSubNode.GetAttribute<float32>( "emission_time"  , 0.0f );
      mCubicSize          = lSubNode.GetAttribute < Math::Vect3f > ( "cubic_size"  , Math::Vect3f() );
      mAngleStep          = lSubNode.GetAttribute<float32>( "angle_step", 0 );
      mCurrentAngle       = 0.0;
      mRadiusMin          = lSubNode.GetAttribute<float32>( "radius_min", 0 );
      mRadiusMax          = lSubNode.GetAttribute<float32>( "radius_max", 0 );
    }
  }

  ASSERT( mMaxAliveParticles <= MAX_PARTICLES, "The maxiumum number of particles is %u it could not be %u", mMaxAliveParticles, MAX_PARTICLES );
  if ( mMaxAliveParticles )
  {
    // Reserve the memory for the particles
    ALLOC_PARTICLES_PROPERTY( mParticlesStream,     TPARTICLE_VERTEX_INSTANCE,  mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesTTL,        float,                      mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesLifeTime,   float,                      mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesAngles,     float,                      mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesSpeed,      float,                      mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesRadialSpeed,float,                      mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesIsAlive,    bool,                       mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesFlipUVHorizontal,    bool,             mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesFlipUVVertical,    bool,                mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesDirections, Math::Vect3f,               mMaxAliveParticles );
    ALLOC_PARTICLES_PROPERTY( mParticlesPositions,  Math::Vect3f,               mMaxAliveParticles );

    mRV = new CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>
      ( GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount, mMaxAliveParticles );
  }
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
  LoadFromNode( atts );
  return true;
}

void CParticleEmitter::Update( float dt )
{
  mActualTime += dt;
  mTimeSinceLastEmission += dt;

  KillParticles();
  EmitParticles();

  for ( uint32 i = 0, lParticles = mMaxAliveParticles; i < lParticles; ++i )
  {
    if ( mParticlesIsAlive[i] )
    {
      mParticlesLifeTime[i] += dt;
      mParticlesDirections[i] *= mParticlesSpeed[i];
      mParticlesDirections[i].y += -mMotion.mGravity * dt;
      mParticlesPositions[i] += mParticlesDirections[i] * dt; 
      mParticlesDirections[i].Normalize();
      mParticlesAngles[i] += dt * mParticlesRadialSpeed[i];

      mParticlesStream[i].alive = Math::Utils::Deg2Rad( mParticlesAngles[i] );

      mInterpolator.Interpolate
      (
      ( mParticlesLifeTime[i] / mParticlesTTL[i] ),
        mParticlesStream[i].size,
        mParticlesStream[i].r,
        mParticlesStream[i].g,
        mParticlesStream[i].b,
        mParticlesStream[i].alpha
      );

      mParticlesStream[i].x = mParticlesPositions[i].x;
      mParticlesStream[i].y = mParticlesPositions[i].y;
      mParticlesStream[i].z = mParticlesPositions[i].z;

      mParticlesStream[i].fliph = static_cast<float>( mParticlesFlipUVHorizontal[i] );
      mParticlesStream[i].flipv = static_cast<float>( mParticlesFlipUVVertical[i] );
    }
    else
    {
      mParticlesStream[i].alive = 0.0;
    }
  }
}

void CParticleEmitter::Render()
{
  CGraphicsManager* lGM = GraphicsInstance;
  ( ( CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>* )mRV )->AddInstancinguffer( lGM, mParticlesStream );
  
  //Activates the textures
  ActivateTextures();

  // Sets the textures for the animation
  CEffect* lEffect = mTechnique->GetEffect();
  if( mTexture.mValue2 )
  {
      float lPercentage = mActualTime / mTTLEmitter;
      lEffect->SetPercentageTextures( 1.0f - lPercentage, lPercentage );
  }
  else
  {
      lEffect->SetPercentageTextures( 1.0f, 0.0f );
  }

  ( ( CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>* )mRV )->Render( lGM, mTechnique );
}

void CParticleEmitter::EmitParticles( )
{
    if ( ( mTimeSinceLastEmission > mEmissionTime ) && IsActive() )
    {
      const uint32 lParticlesToEmit = baseUtils::RandRange( mParticlesPerEmission.x, mParticlesPerEmission.y );

      for ( uint32 i = 0, lEmittedParticles = 0; i < mMaxAliveParticles &&
            mAliveParticlesCount < mMaxAliveParticles && lEmittedParticles < lParticlesToEmit ; ++i )
      {
        if ( !mParticlesIsAlive[i] )
        {
          mParticlesIsAlive[i] =  true;
          mParticlesSpeed[i] = baseUtils::RandRange( mMotion.mLinearSpeed.x, mMotion.mLinearSpeed.y );
          
          if( mUseFixedDirection )
            mParticlesDirections[i] = mFixedDirection;
          else
            mParticlesDirections[i]= baseUtils::RandRange( mMotion.mInitialDirectionMin, mMotion.mInitialDirectionMax );

          mParticlesTTL[i] = baseUtils::RandRange( mTTLParticles.x, mTTLParticles.y );
          mParticlesPositions[i] = mSpawnFn( this );

          mParticlesRadialSpeed[i] = baseUtils::RandRange( mMotion.mRadialSpeed.x, mMotion.mRadialSpeed.y );
          mParticlesAngles[i] = ( mRandomInitialAngle ) ? RandRange( 0.0f, 360.0f ) : 0.01f;

          mParticlesFlipUVHorizontal[i] = (mTexture.mFlipUVHorizontal) ? baseUtils::RandomBool() : false;
          mParticlesFlipUVVertical[i]  = (mTexture.mFlipUVVertical)   ? baseUtils::RandomBool() : false;

          ++mAliveParticlesCount;
          ++lEmittedParticles;
        }
      }

      mTimeSinceLastEmission = 0.0f;
    }
}

void CParticleEmitter::KillParticles()
{
  for ( uint32 i = 0; i < mMaxAliveParticles && mAliveParticlesCount > 0; ++i )
  {
    if ( mParticlesIsAlive[i] && mParticlesLifeTime[i] > mParticlesTTL[i] )
    {
      mParticlesIsAlive[i] = false;
      mParticlesLifeTime[i] = 0.0f;
      --mAliveParticlesCount;
    }
  }
}

void CParticleEmitter::ActivateTextures()
{
    mTexture.mValue->Activate( 0 );

    if( mTexture.mValue2 )
        mTexture.mValue2->Activate( 1 );
}

//-----------------------------------------------------------------------------------------
bool CParticleEmitter::IsActive()
{
  if( mActualTime > mTTLEmitter )
  {
    if( mIsLoop )
      mActualTime = 0.0f;
    else
      mIsActive = false;
  }

  return mIsActive;
}

void CParticleEmitter::Reset()
{
  mActualTime = 0.0f;
  mIsActive = true;
  mAliveParticlesCount = 0.0;
  
  for ( uint32 i = 0; i < mMaxAliveParticles; ++i )
  {
      mParticlesIsAlive[i] = false;
      mParticlesLifeTime[i] = 0.0f;
  }
}

void CParticleEmitter::SetFixedDirection( const Math::Vect3f& aDirection )
{
  mUseFixedDirection = true;
  mFixedDirection = aDirection;
}