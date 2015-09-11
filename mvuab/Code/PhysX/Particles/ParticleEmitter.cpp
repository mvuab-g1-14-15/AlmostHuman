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
    , mRV(0)
    , mType(aType)
    , mSpawnFn(aFunction)
    , mIsLoop( false )
    , mIsActive( false )
    , mAliveParticlesCount(0)
    , mEmissionTime(Math::Vect2f(1.0f, 1.0f))
    , mAliveParticles( Math::Vect2u( 0, 0 ) )
    , mTimeToLive(Math::Vect2f(1.0f, 1.0f))
    , mActualTime(0.0f)
    , mSize(Math::Vect2f(1.0f, 1.0f))
    , mSpeed(Math::Vect2f(1.0f, 1.0f))
    , mTechnique( 0 )
    , mIsImmortal( 0 )
    , mParticlesXEmission(Math::Vect2f(1.0f, 1.0f))
    , mGravity(0.f)
    , mParticles( 0 )
    , mMinPnt( Math::Vect3f(1.0f, 1.0f, 1.0f) )
    , mMaxPnt( Math::Vect3f(1.0f, 1.0f, 1.0f) )
    , mAlpha( 0.0f )
{
}

CParticleEmitter::~CParticleEmitter()
{
    CHECKED_DELETE_ARRAY(mParticlesStream);
    CHECKED_DELETE_ARRAY(mParticles);

    CHECKED_DELETE(mShape);
    CHECKED_DELETE(mRV);
}

void CParticleEmitter::LoadFromNode( const CXMLTreeNode& atts )
{
  SetName(atts.GetAttribute<std::string>("name", "no_name"));
    mIsLoop               = atts.GetAttribute<bool>("loop", false );
    mIsImmortal           = atts.GetAttribute<bool>("immortal", false );
    mIsActive             = atts.GetAttribute<bool>("active", false );
    mAliveParticles       = atts.GetAttribute<Math::Vect2u>("alive_particles", 0);
    mSpeed                = atts.GetAttribute<Math::Vect2f>("speed", 0);
    mEmissionTime         = atts.GetAttribute<Math::Vect2f>("emission_time", Math::Vect2f(1.0f, 1.0f));
    mTimeToLive           = atts.GetAttribute<Math::Vect2f>("time_to_live", Math::Vect2f(1.0f, 1.0f));
    mSize                 = atts.GetAttribute<Math::Vect2f>("particle_size_range", 0.0f );
    mTechnique            = atts.GetAttribute<CEffectTechnique>("technique");
    mAlpha                = atts.GetAttribute<float32>("alpha", 1.0f);
    mParticlesXEmission   = atts.GetAttribute<Math::Vect2f>("particles_per_emission", Math::Vect2f(1.0f, 1.0f) );
    mColorMin             = atts.GetAttribute<Math::CColor>("min_color", Math::colWHITE );
    mColorMax             = atts.GetAttribute<Math::CColor>("max_color", mColorMin );
    mInitialDirectionMin  = atts.GetAttribute<Math::Vect3f>("min_initial_direction", Math::Vect3f());
    mInitialDirectionMax  = atts.GetAttribute<Math::Vect3f>("max_initial_direction", mInitialDirectionMin );
    mGravity              = atts.GetAttribute<float32>("gravity", 1.0f );
    mRadialSpeed          = atts.GetAttribute<Math::Vect2f>("radial_speed", 1.0f );
    mOndSpeed             = atts.GetAttribute<Math::Vect2f>("ondulative_speed", Math::Vect2f(1.0f, 1.0f) );
    mOndSpeedDirectionMin = atts.GetAttribute<float32>("min_ondulative_direction", 0.0);
    mOndSpeedDirectionMax = atts.GetAttribute<float32>("max_ondulative_direction", mOndSpeedDirectionMin );

    // Box Emitter
    Math::Vect3f lSize = atts.GetAttribute<Math::Vect3f>("cubic_size", Math::Vect3f());
    mMinPnt = -lSize;
    mMaxPnt =  lSize;

    // Get textures
    for( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lSubNode = atts(i);
        const std::string& lTagName = lSubNode.GetName();
        if( lTagName == "texture" )
        {
            CTexture* lTexture = lSubNode.GetAttribute<CTexture>("name");
            if( lTexture )
            {
                mTextures.push_back(lTexture);
            }
        }
    }

    // Reserve the memory for the particles
    mParticles = new CParticle[mAliveParticles.y];
    ZeroMemory(mParticles, mAliveParticles.y * sizeof(CParticle) );

    mParticlesStream = new TPARTICLE_VERTEX_INSTANCE[mAliveParticles.y];
    ZeroMemory(mParticles, mAliveParticles.y * sizeof(TPARTICLE_VERTEX_INSTANCE) );

    mRV = new CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>(GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount, mAliveParticles.y );
    mShape = new CBoxShape();
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
  const std::string& lFile = atts.GetAttribute<std::string>("file", "" );
  if( lFile != "" )
  {
    CXMLTreeNode lFileNode, lEmitterNode;
    if( lFileNode.LoadAndFindNode( lFile.c_str(), "emitter", lEmitterNode ) )
      LoadFromNode( lEmitterNode );
  }
  else
  {
    LoadFromNode( atts );
  }

  return true;
}

void CParticleEmitter::Update( float dt )
{
    KillParticles();
    EmitParticles();
    for( uint32 i = 0, lParticles = mAliveParticles.y; i < lParticles; ++i)
    {
        if(mParticles[i].IsAlive())
        {
            CParticle& lParticle = mParticles[i];
            lParticle.Update(dt);
            mParticlesStream[i].alive = Math::Utils::Deg2Rad( lParticle.GetAngle());
            const float32 lPercentage = lParticle.GetActualTime() / lParticle.GetTimeToLive();
            mParticlesStream[i].size = Math::Utils::Lerp<float32>(mSize.x, mSize.y, lPercentage);//lParticle.GetSize();
            
            mParticlesStream[i].alpha= lParticle.GetAlpha();
            if( lPercentage > 0.90f )
              mParticlesStream[i].alpha = 1.0f - lPercentage;
            else if( lPercentage < 0.10f )
              mParticlesStream[i].alpha = lPercentage;
            
            mParticlesStream[i].x = lParticle.GetPosition().x;
            mParticlesStream[i].y = lParticle.GetPosition().y;
            mParticlesStream[i].z = lParticle.GetPosition().z;
            mParticlesStream[i].r = lParticle.GetColor().r;
            mParticlesStream[i].g = lParticle.GetColor().g;
            mParticlesStream[i].b = lParticle.GetColor().b;
        }
        else
        {
            mParticlesStream[i].alive = 0.0;
        }
    }
}

void CParticleEmitter::Render()
{
    //mShape->Render( EffectManagerInstance->GetEffectTechnique("RenderForwardDebugShapeTechnique") );
    CGraphicsManager* lGM = GraphicsInstance;

    ((CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE> *)mRV)->AddInstancinguffer(lGM, mParticlesStream);
    ActivateTextures();

    ((CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE> *)mRV)->Render(lGM, mTechnique);
}

void CParticleEmitter::EmitParticles()
{
  if( IsActive() )
  {
    const uint32 lParticlesToEmit = (uint32)RandRange( mParticlesXEmission.x, mParticlesXEmission.y );
    for( uint32 i = 0, lEmittedParticles = 0; i < mAliveParticles.y &&
            mAliveParticlesCount < mAliveParticles.y && lEmittedParticles < lParticlesToEmit ; ++i )
    {
        CParticle& lParticle = mParticles[i];

        if( !lParticle.IsAlive() )
        {
            lParticle.SetIsAlive(true);
            lParticle.SetGravity( mGravity );
            lParticle.SetSpeed(RandRange(mSpeed.x, mSpeed.y));
            lParticle.SetDirection(RandRange(mInitialDirectionMin, mInitialDirectionMax).Normalize());
            lParticle.SetSize(RandRange(mSize.x, mSize.y));
            lParticle.SetTimeToLive(RandRange(mTimeToLive.x, mTimeToLive.y));
            lParticle.SetPosition(mSpawnFn(this));
            lParticle.SetOndSpeed(RandRange(mOndSpeed.x, mOndSpeed.y));
            lParticle.SetOndSpeedDirection(RandRange(mOndSpeedDirectionMin, mOndSpeedDirectionMax));
            lParticle.SetRadialSpeed(RandRange(mRadialSpeed.x, mRadialSpeed.y));
            lParticle.SetColor(RandRange(mColorMin, mColorMax));
            lParticle.SetInitalOndulation(RandRange(0.0f, 360.0f));
            lParticle.SetOndulationVel(RandRange(mOndSpeedDirectionMin, mOndSpeedDirectionMax));
            lParticle.SetAngle(RandRange(0.0f, 360.0f));
            lParticle.SetAlpha( RandRange(0.0f, mAlpha) );
            ++mAliveParticlesCount;
            ++lEmittedParticles;
        }
    }
  }
}

void CParticleEmitter::KillParticles()
{
    for( uint32 i = 0; i < mAliveParticles.y && mAliveParticlesCount > 0; ++i )
    {
        CParticle& lParticle = mParticles[i];
        if( lParticle.IsAlive() && lParticle.GetActualTime() > lParticle.GetTimeToLive() )
        {
            lParticle.SetIsAlive(false);
            lParticle.SetActualTime(0.0f);
            --mAliveParticlesCount;
        }
    }
}

void CParticleEmitter::ActivateTextures()
{
    for( uint32 i = 0, lCount = mTextures.size(); i < lCount; ++i )
    {
        mTextures[i]->Activate(i);
    }
}