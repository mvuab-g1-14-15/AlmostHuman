#include "SphereEmitter.h"

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
        


    }
}

void CSphereEmitter::NewParticleSphere(CParticle *l_Particle)
{

    /*float inclination = Math::Utils::Deg2Rad( RandRange( MinInclination, MaxInclination ) );
    float azimuth = glm::radians( RandRange( MinAzimuth, MaxAzimuth ) );

    float radius = RandRange( MinimumRadius, MaximumRadius );
    float speed = RandRange( MinSpeed, MaxSpeed );
    float lifetime = RandRange( MinLifetime, MaxLifetime );

    float sInclination = sinf( inclination );

    float X = sInclination * cosf( azimuth );
    float Y = sInclination * sinf( azimuth );
    float Z = cosf( inclination );

    glm::vec3 vector( X, Y, Z );

    particle.m_Position = ( vector * radius ) + Origin;
    particle.m_Velocity = vector * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;*/

}