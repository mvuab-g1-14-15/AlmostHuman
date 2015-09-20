#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "Texture/Texture.h"
#include "EngineManagers.h"
#include "Effects/EffectManager.h"
#include "Billboard/Billboard.h"
#include "RenderableVertex/InstancedVertexTypes.h"
#include "RenderableVertex/InstancingVertexs.h"
#include "Shapes/Shapes.h"

#define MAX_PARTICLES 400
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


CParticleEmitter::CParticleEmitter( ps::TEmitterType aType, ps::TSpawnFunction aFunction )
  : CName()
  , mRV( 0 )
  , mType( aType )
  , mSpawnFn( aFunction )
  , mIsLoop( false )
  , mIsActive( false )
  , mAliveParticlesCount( 0 )
  , mParticles( 0 )
  , mTTLEmitter( 0.0f )
  , mTechnique( NULL )
  , mRandomInitialAngle( false )
  , mEmissionTime( 0.0f )
  , mTimeSinceLastEmission( 0.0f )
  , mMaxLife( 0.0f )
  , mActualTime( 0.0f )
{
}

CParticleEmitter::~CParticleEmitter()
{
  CHECKED_DELETE_ARRAY( mParticlesStream );
  CHECKED_DELETE_ARRAY( mParticles );
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
    }
  }

  ASSERT( mMaxAliveParticles <= MAX_PARTICLES, "The maxiumum number of particles is %u it could not be %u", mMaxAliveParticles, MAX_PARTICLES );
  if ( mMaxAliveParticles )
  {
    // Reserve the memory for the particles
    mParticles = new CParticle[mMaxAliveParticles];

    ZeroMemory( mParticles, mMaxAliveParticles * sizeof( CParticle ) );

    mParticlesStream = new TPARTICLE_VERTEX_INSTANCE[mMaxAliveParticles];
    ZeroMemory( mParticles, mMaxAliveParticles * sizeof( TPARTICLE_VERTEX_INSTANCE ) );

    mRV = new CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>( GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount,
        mMaxAliveParticles );
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

  if ( mActualTime > mTTLEmitter )
  {
    mIsActive = false;
  }

  KillParticles();
  EmitParticles();

  for ( uint32 i = 0, lParticles = mMaxAliveParticles; i < lParticles; ++i )
  {
    if ( mParticles[i].IsAlive() )
    {
      CParticle& lParticle = mParticles[i];
      lParticle.Update( dt );
      mParticlesStream[i].alive = Math::Utils::Deg2Rad( lParticle.GetAngle() );

      mInterpolator.Interpolate
      (
        ( lParticle.GetActualTime() / lParticle.GetTimeToLive() ),
        mParticlesStream[i].size,
        mParticlesStream[i].r,
        mParticlesStream[i].g,
        mParticlesStream[i].b,
        mParticlesStream[i].alpha
      );

      mParticlesStream[i].x = lParticle.GetPosition().x;
      mParticlesStream[i].y = lParticle.GetPosition().y;
      mParticlesStream[i].z = lParticle.GetPosition().z;

      mParticlesStream[i].fliph = static_cast<float>( lParticle.GetFLipUVHorizontal() );
      mParticlesStream[i].flipv = static_cast<float>( lParticle.GetFlipUVVertical() );
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
  ActivateTextures();
  ( ( CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>* )mRV )->Render( lGM, mTechnique );
}

void CParticleEmitter::EmitParticles( )
{
  if (mIsActive)
  {
    if ( ( mTimeSinceLastEmission > mEmissionTime ) && IsActive() )
    {
      const uint32 lParticlesToEmit = baseUtils::RandRange( mParticlesPerEmission.x, mParticlesPerEmission.y );

      for ( uint32 i = 0, lEmittedParticles = 0; i < mMaxAliveParticles &&
            mAliveParticlesCount < mMaxAliveParticles && lEmittedParticles < lParticlesToEmit ; ++i )
      {
        CParticle& lParticle = mParticles[i];

        if ( !lParticle.IsAlive() )
        {
          lParticle.SetIsAlive( true );
          lParticle.SetGravity( mMotion.mGravity );
          lParticle.SetSpeed( baseUtils::RandRange( mMotion.mLinearSpeed.x, mMotion.mLinearSpeed.y ) );
          lParticle.SetDirection( baseUtils::RandRange( mMotion.mInitialDirectionMin, mMotion.mInitialDirectionMax ) );

          lParticle.SetTimeToLive( baseUtils::RandRange( mTTLParticles.x, mTTLParticles.y ) );
          lParticle.SetPosition( mSpawnFn( this ) );

          lParticle.SetRadialSpeed( baseUtils::RandRange( mMotion.mRadialSpeed.x, mMotion.mRadialSpeed.y ) );

          if( mRandomInitialAngle )
            lParticle.SetAngle( RandRange( 0.0f, 360.0f ) );
          else
            lParticle.SetAngle( 0.0f );

          lParticle.SetFlipUV
                (
                  (mTexture.mFlipUVHorizontal) ? baseUtils::RandomBool() : false,
                  (mTexture.mFlipUVVertical)   ? baseUtils::RandomBool() : false
                );

          ++mAliveParticlesCount;
          ++lEmittedParticles;
        }
      }

      mTimeSinceLastEmission = 0.0f;
    }
  }
}

void CParticleEmitter::KillParticles()
{
  for ( uint32 i = 0; i < mMaxAliveParticles && mAliveParticlesCount > 0; ++i )
  {
    CParticle& lParticle = mParticles[i];

    if ( lParticle.IsAlive() && lParticle.GetActualTime() > lParticle.GetTimeToLive() )
    {
      lParticle.SetIsAlive( false );
      lParticle.SetActualTime( 0.0f );
      --mAliveParticlesCount;
    }
  }
}

void CParticleEmitter::ActivateTextures()
{
  mTexture.mValue->Activate( 0 );
}