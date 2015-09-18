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

namespace
{
	ps::EmitterSpeedPropery GetSpeedFromNode( const CXMLTreeNode& aNode )
	{
		ps::EmitterSpeedPropery lSpeed = 
		{
			aNode.GetAttribute<Math::Vect2f>("value", Math::Vect2f(0.0f, 0.0f) ),
			aNode.GetAttribute<Math::Vect3f>("min_dir", Math::Vect3f(0.0f, 0.0f, 0.0f ) ),
			aNode.GetAttribute<Math::Vect3f>("max_dir", Math::Vect3f(0.0f, 0.0f, 0.0f ) ),
		};

		return lSpeed;
	}

	ps::ParticleVisualProperty GetVisualProperyFromNode( const CXMLTreeNode& aNode )
	{
		ps::ParticleVisualProperty lVisualProperty;

		// Fill the other data
		lVisualProperty.mAlpha		= aNode.GetAttribute<float>("alpha", 0.0f);
		lVisualProperty.mMinColor   = aNode.GetAttribute<Math::CColor>("min_color", Math::CColor(0.0f, 0.0f, 0.0f, 0.0f ) );
		lVisualProperty.mMaxColor   = aNode.GetAttribute<Math::CColor>("max_color", Math::CColor(0.0f, 0.0f, 0.0f, 0.0f ) );
		lVisualProperty.mKeepSize   = aNode.GetAttribute<bool>("keep_size", false);
		lVisualProperty.mSizeRange  = aNode.GetAttribute<Math::Vect2f>("size_range", Math::Vect2f(0.0f, 0.0f) );
		lVisualProperty.mTechnique  = aNode.GetAttribute<CEffectTechnique>("technique");

		return lVisualProperty;
	}

	ps::Emissions GetEmissionsFromNode( const CXMLTreeNode& aNode )
	{
		ps::Emissions lEmissions = 
		{
			aNode.GetAttribute<float>("emission_time", 0.0f),
			aNode.GetAttribute<bool>("modify_initial_angle", false),
			aNode.GetAttribute<Math::Vect2u>("particles_per_emission", Math::Vect2u(0, 0) ),
			aNode.GetAttribute<Math::Vect3f>("cubic_size", Math::Vect3f(0.0f, 0.0f, 0.0f ) ),
			aNode.GetAttribute<float>("min_radius", 0.0f),
			aNode.GetAttribute<float>("max_radius", 0.0f),
			aNode.GetAttribute<float>("angle_step", 0.0f),
		};

		return lEmissions;
	}

	ps::ParticlesProperties GetParticlePropertiesFromNode( const CXMLTreeNode& aNode )
	{
		ps::ParticlesProperties lParticleProps;

		for( uint32 i = 0, lCount = aNode.GetNumChildren(); i < lCount; ++i )
		{
			const CXMLTreeNode& lNode = aNode(i);
			if( lNode.GetName() == "texture" )
			{
				lParticleProps.mTexture						= ps::ParticleTexture();
				lParticleProps.mTexture.mValue				= lNode.GetAttribute<CTexture>("name");
				lParticleProps.mTexture.mFlipUVHorizontal  = lNode.GetAttribute<bool>("flip_uv_horizontal", false);
				lParticleProps.mTexture.mFlipUVVertical    = lNode.GetAttribute<bool>("flip_uv_vertical"  , false);
			}
		}

		return lParticleProps;
	}
}

CParticleEmitter::CParticleEmitter( ps::TEmitterType aType, ps::TSpawnFunction aFunction )
    : CName()
    , mRV(0)
    , mType(aType)
    , mSpawnFn(aFunction)
    , mIsLoop( false )
    , mIsActive( false )
    , mAliveParticlesCount(0)
    , mActualTime(0.0f)
    , mGravity(0.f)
    , mParticles( 0 )
	, mTimeSinceLastEmission(0.0f)
	, mMaxLife(0.0f)
{
}

CParticleEmitter::~CParticleEmitter()
{
    CHECKED_DELETE_ARRAY(mParticlesStream);
    CHECKED_DELETE_ARRAY(mParticles);
    CHECKED_DELETE(mRV);
}

void CParticleEmitter::LoadFromNode( const CXMLTreeNode& atts )
{
    SetName(atts.GetAttribute<std::string>("name", "no_name"));
    mIsLoop               = atts.GetAttribute<bool>("loop", false );
    mIsActive             = atts.GetAttribute<bool>("active", false );

    for( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lSubNode = atts(i);
        const std::string& lTagName = lSubNode.GetName();
        if( lTagName == "speeds" )
		{
			for( uint32 iSpeed = 0, lSpeedCount = lSubNode.GetNumChildren() ; iSpeed < lSpeedCount; ++iSpeed )
			{
				const CXMLTreeNode& lSpeedNode = lSubNode(iSpeed);
				const std::string& lSpeedTag = lSubNode.GetName();
				if( lSpeedTag == "linear" )
				{
					mLinearSpeed = GetSpeedFromNode( lSpeedNode );
				}
				else if ( lSpeedTag == "radial" )
				{
					mRadialSpeed = GetSpeedFromNode( lSpeedNode );
				}
				else if ( lSpeedTag == "ondulative" )
				{
					mOndulativeSpeed = GetSpeedFromNode( lSpeedNode );
				}
				else if ( lSpeedTag == "gravity" )
				{
					mGravity = lSpeedNode.GetAttribute<float>("value", 1.0f);
				}
			}
		}
		else if( lTagName == "particles" )
		{
			mParticleProperies = GetParticlePropertiesFromNode( lSubNode );
		}
    }

    // Reserve the memory for the particles
	mParticles = new CParticle[mParticleProperies.mMaxAliveParticles];
    ZeroMemory(mParticles, mParticleProperies.mMaxAliveParticles * sizeof(CParticle) );

    mParticlesStream = new TPARTICLE_VERTEX_INSTANCE[mParticleProperies.mMaxAliveParticles];
    ZeroMemory(mParticles, mParticleProperies.mMaxAliveParticles * sizeof(TPARTICLE_VERTEX_INSTANCE) );

    mRV = new CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE>(GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount, mParticleProperies.mMaxAliveParticles );
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
	mTimeSinceLastEmission += dt;
	mActualTime += dt;

    KillParticles();
    EmitParticles();
	for( uint32 i = 0, lParticles = mParticleProperies.mMaxAliveParticles; i < lParticles; ++i)
    {
        if(mParticles[i].IsAlive())
        {
            CParticle& lParticle = mParticles[i];
            lParticle.Update(dt);
            mParticlesStream[i].alive = Math::Utils::Deg2Rad( lParticle.GetAngle());
            const float32 lPercentage = lParticle.GetActualTime() / lParticle.GetTimeToLive();

			mParticlesStream[i].size = ( mParticleProperies.KeepSize() ) ? lParticle.GetSize() : mParticleProperies.Size(lPercentage);
            
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
            mParticlesStream[i].fliph = static_cast<float>( lParticle.GetFLipUVHorizontal () ); 
            mParticlesStream[i].flipv = static_cast<float>( lParticle.GetFlipUVVertical () ); 
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
    ((CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE> *)mRV)->AddInstancinguffer(lGM, mParticlesStream);
    ActivateTextures();
	((CInstancingVertexs<TPARTICLE_VERTEX, TPARTICLE_VERTEX_INSTANCE> *)mRV)->Render(lGM, mParticleProperies.mVisualProperty.mTechnique );
}

void CParticleEmitter::EmitParticles()
{
  if( ( mActualTime < mTimeSinceLastEmission ) && IsActive() )
  {
	  const uint32 lParticlesToEmit = mEmissions.GetParticlesToEmit();
	  for( uint32 i = 0, lEmittedParticles = 0; i < mParticleProperies.mMaxAliveParticles && mAliveParticlesCount < mParticleProperies.mMaxAliveParticles && lEmittedParticles < lParticlesToEmit ; ++i )
	  {
        CParticle& lParticle = mParticles[i];

        if( !lParticle.IsAlive() )
        {
            lParticle.SetIsAlive(true);
            lParticle.SetGravity( mGravity );
			lParticle.SetSpeed( mLinearSpeed.GetRange() );
			lParticle.SetDirection( mLinearSpeed.GetRandomDirection() );
			lParticle.SetSize( mParticleProperies.Size() );
			lParticle.SetTimeToLive( mParticleProperies.MaxLife() );
			lParticle.SetPosition( mSpawnFn(this) );

			lParticle.SetRadialSpeed( mRadialSpeed.GetRange() );
			lParticle.SetColor(mParticleProperies.Color());
            
            lParticle.SetAngle(RandRange(0.0f, 360.0f));
			lParticle.SetAlpha( mParticleProperies.Alpha() );
			lParticle.SetFlipUV( mParticleProperies.FlipUVHorizontal(), mParticleProperies.FlipUVVertical() );
                ++mAliveParticlesCount;
            ++lEmittedParticles;
        }
    }

	mTimeSinceLastEmission = 0.0f;
  }
}

void CParticleEmitter::KillParticles()
{
	for( uint32 i = 0; i < mParticleProperies.mMaxAliveParticles && mAliveParticlesCount > 0; ++i )
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
	mParticleProperies.ActivateTexture();
}