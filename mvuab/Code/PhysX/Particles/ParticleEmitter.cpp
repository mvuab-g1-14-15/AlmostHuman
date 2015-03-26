#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"



CParticleEmitter::CParticleEmitter()
{
    m_Particles.resize(0);

	m_Max = 0;
	m_Min = 0;
	
	m_ActualTime = 0.0f;
    
    m_MinLifetime = 0;
    m_MaxLifetime = 0;

    m_Position = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Direction = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Velocity = Math::Vect3f(0.0f, 0.0f, 0.0f);
}

CParticleEmitter::~CParticleEmitter()
{
    m_Particles.clear();
}

void CParticleEmitter::SetAcceleration(const Math::Vect3f &Acceleration)
{
    m_Acceleration = Acceleration;
}

void CParticleEmitter::SetDirection(const Math::Vect3f &Direction)
{
    m_Direction = Direction;
}

void CParticleEmitter::SetPosition(const Math::Vect3f &Position)
{
    m_Position = Position;
}
void CParticleEmitter::SetVelocity(const Math::Vect3f &Velocity)
{
    m_Velocity = Velocity;
}

void CParticleEmitter::SetLifeTime(float min, float max)
{
    m_MinLifetime = min;
    m_MaxLifetime =  max;
}
