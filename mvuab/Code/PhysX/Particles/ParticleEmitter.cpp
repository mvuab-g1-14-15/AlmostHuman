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
    , mTechnique(0)
{
    m_Particles.resize( 0 );

    m_MinLifetime = 0.0f;
    m_MaxLifetime = 0.0f;
    m_TimeToEmit = 0.0f;

    m_Rand = 0.0f;
    m_RandMin = 0.0f;
    m_RandMax = 0.0f;


    m_Position = Math::Vect3f( 0.0f, 0.0f, 0.0f );
    m_Direction = Math::Vect3f( 0.0f, 0.0f, 0.0f );
    m_Velocity = Math::Vect3f( 0.0f, 0.0f, 0.0f );

    m_Active = true;
    m_Orientate = false;
    m_ActualTime = 0.0f;
}

CParticleEmitter::~CParticleEmitter()
{
    m_Particles.clear();
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
    mIsLoop       = atts.GetBoolProperty("loop", false );
    mIsActive     = atts.GetBoolProperty("active", false );
    mMaxParticles = atts.GetIntProperty("max_particles", 0);
    mTimeToEmit   = atts.GetFloatProperty("time_to_emit", 0.0f);
    mTimeToLive   = atts.GetFloatProperty("time_to_live", 0.0f);
    mSize         = atts.GetVect2fProperty("size", 0.0f );
    mAcceleration = atts.GetVect3fProperty("acceleration", Math::Vect3f());
    mDirection    = atts.GetVect3fProperty("direction", Math::Vect3f());
    mPosition     = atts.GetVect3fProperty("position", Math::Vect3f());
    mVelocity     = atts.GetVect3fProperty("velocity", Math::Vect3f());
    mTechnique    = EffectManagerInstance->GetEffectTechnique( atts.GetPszProperty("technique", "none") );
    ASSERT( mTechnique, "Null technique to render the emitter");

    return true;
}

void CParticleEmitter::SetAcceleration( const Math::Vect3f& Acceleration )
{
    mAcceleration = Acceleration;
}

void CParticleEmitter::SetDirection( const Math::Vect3f& Direction )
{
    mDirection = Direction;
}

void CParticleEmitter::SetPosition( const Math::Vect3f& Position )
{
    mPosition = Position;
}
void CParticleEmitter::SetVelocity( const Math::Vect3f& Velocity )
{
    mVelocity = Velocity;
}

void CParticleEmitter::SetLifeTime( float min, float max )
{
    m_MinLifetime = min;
    m_MaxLifetime =  max;
}

void CParticleEmitter::SetSize(float sx, float sy)
{
    m_SizeX = sx;
    m_SizeY = sy;
}

void CParticleEmitter::SetTimeToEmit(float Time)
{
    m_TimeToEmit = Time;
}

float CParticleEmitter::GetTimeToEmit()
{
    return m_TimeToEmit;
}

void CParticleEmitter::SetEmitterLifeTime(float LifeTime)
{
    m_EmitterLifeTime = LifeTime;
}

float CParticleEmitter::GetActive()
{
    return m_Active;
}

void CParticleEmitter::SetActive(bool Active)
{
    m_Active = Active;
}

void CParticleEmitter::SetTextureName(const std::string &Texture)
{
    m_TextureName = Texture;
}

void CParticleEmitter::SetOrientate(bool l_Orientate)
{
    m_Orientate = l_Orientate;
}
