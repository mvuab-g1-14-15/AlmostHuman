#include "SphereEmitter.h"
#include "Utils/BaseUtils.h"

CSphereEmitter::CSphereEmitter()
{
    m_MinimumRadius = 0.0f;
    m_MaximumRadius = 0.0f;

    m_MinPitch = 0.0f;
    m_MaxPitch = 0.0f;

    m_MinYaw = 0.0f;
    m_MaxYaw = 0.0f;
}

CSphereEmitter::~CSphereEmitter()
{
}

void CSphereEmitter::SetRadius(float min, float max)
{
    m_MinimumRadius = min;
    m_MaximumRadius = max;
}

void CSphereEmitter::SetPitch(float min, float max)
{
    m_MinPitch = min;
    m_MaxPitch = max;
}

void CSphereEmitter::SetYaw(float min, float max)
{
    m_MinYaw = min;
    m_MaxYaw = max;
}

void CSphereEmitter::Generate (unsigned int l_NumParticles)
{
    m_Particles.resize(l_NumParticles);

    for(unsigned int i = 0; i < l_NumParticles; ++i)
    {
        NewParticleSphere(&m_Particles[i]);
    }
}

void CSphereEmitter::NewParticleSphere(CParticle *l_Particle)
{
    float l_Pitch = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxPitch));
    float l_Yaw = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxYaw));

    float l_Radius = baseUtils::RandRange(m_MinimumRadius, m_MaximumRadius);
    float l_LifeTime = baseUtils::RandRange(m_MinLifetime, m_MaxLifetime);

    float x =  Math::Utils::Cos(l_Yaw) *  Math::Utils::Cos(l_Yaw + Math::pi32 * 0.5f);
    float y =  Math::Utils::Sin(l_Pitch);
    float z =   Math::Utils::Sin(l_Yaw) + Math::Utils::Sin(l_Yaw + Math::pi32 * 0.5f);

    Math::Vect3f l_Vector = Math::Vect3f(x, y, z).Normalize();
	l_Vector.x *= m_Velocity.x;
	l_Vector.y *= m_Velocity.y;
	l_Vector.z *= m_Velocity.z;

    l_Particle->SetVelocity(l_Vector);
    l_Particle->SetAcceleration(m_Acceleration);

    l_Particle->SetPosition(m_Position);
    l_Particle->SetLifeTime(l_LifeTime);
    l_Particle->SetIsAlive(true);
}