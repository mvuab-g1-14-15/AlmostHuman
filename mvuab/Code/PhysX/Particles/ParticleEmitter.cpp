#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"

CParticleEmitter::CParticleEmitter()
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
    m_ActualTime = 0.0f;
}

CParticleEmitter::~CParticleEmitter()
{
    m_Particles.clear();
}

void CParticleEmitter::SetAcceleration( const Math::Vect3f& Acceleration )
{
    m_Acceleration = Acceleration;
}

void CParticleEmitter::SetDirection( const Math::Vect3f& Direction )
{
    m_Direction = Direction;
}

void CParticleEmitter::SetPosition( const Math::Vect3f& Position )
{
    m_Position = Position;
}
void CParticleEmitter::SetVelocity( const Math::Vect3f& Velocity )
{
    m_Velocity = Velocity;
}

void CParticleEmitter::SetLifeTime( float min, float max )
{
    m_MinLifetime = min;
    m_MaxLifetime =  max;
}

void CParticleEmitter::SetSize(float min, float max)
{
    m_MinimumSize = min;
    m_MaximumSize = max;
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
