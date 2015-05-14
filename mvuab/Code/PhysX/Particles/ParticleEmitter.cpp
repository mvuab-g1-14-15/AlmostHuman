#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "EngineManagers.h"
#include "Effects/EffectManager.h"

CParticleEmitter::CParticleEmitter()
    : CName()
    , mIsLoop( false )
    , mIsActive( false )
    , mMaxParticles(0)
    , mParticlesCount(0)
    , mTimeToEmit(0.0f)
    , mTimeToLive(0.0f)
    , mActualTime(0.0f)
    , mSize(Math::Vect2f(1.0f, 1.0f))
    , mAcceleration(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mDirection(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mPosition(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mVelocity(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mTechnique( 0 )
	, mIsInmortal( 0 )
{
}

CParticleEmitter::~CParticleEmitter()
{
	Destroy();
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
    mIsLoop       = atts.GetBoolProperty("loop", false );
	mIsInmortal   = !mIsLoop;
    mIsActive     = atts.GetBoolProperty("active", false );
    mMaxParticles = atts.GetIntProperty("max_particles", 0);
    mTimeToEmit   = atts.GetFloatProperty("time_to_emit", 0.0f);
    mTimeToLive   = atts.GetFloatProperty("time_to_live", 0.0f);
    mSize         = atts.GetVect2fProperty("particle_size_range", 0.0f );
    mAcceleration = atts.GetVect3fProperty("acceleration", Math::Vect3f());
    mDirection    = atts.GetVect3fProperty("direction", Math::Vect3f());
    mPosition     = atts.GetVect3fProperty("position", Math::Vect3f());
    mVelocity     = atts.GetVect3fProperty("velocity", Math::Vect3f());
    mTechnique    = EffectManagerInstance->GetEffectTechnique( atts.GetPszProperty("technique", "none") );
    ASSERT( mTechnique, "Null technique to render the emitter");

    return true;
}

void CParticleEmitter::Update( float dt )
{
	for( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i)
    { 
		CParticle* lParticle = GetResourceById( i );
		lParticle->Update( dt );
    }
}

void CParticleEmitter::Render()
{
	for( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i)
    { 
		CParticle* lParticle = GetResourceById( i );
		lParticle->Render();
    }
}