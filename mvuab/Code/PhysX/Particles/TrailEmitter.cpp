#include "TrailEmitter.h"
#include "Utils/BaseUtils.h"

#include <omp.h>

CTrailEmitter::CTrailEmitter()
{
    float m_MinWidth = 0.0f;
    float m_MaxWidth = 0.0f;

    float m_MinHeight = 0.0f;
    float m_MaxHeight = 0.0f;

    float m_MinDepth = 0.0f;
    float m_MaxDepth = 0.0f;
}

CTrailEmitter::~CTrailEmitter()
{
}

void CTrailEmitter::SetSize(float sx, float sy)
{
    m_SizeX = sx;
    m_SizeY = sy;
}

void CTrailEmitter::SetPitch(float min, float max)
{
    m_MinPitch = min;
    m_MaxPitch = max;
}

void CTrailEmitter::SetYaw(float min, float max)
{
    m_MinYaw = min;
    m_MaxYaw = max;
}

void CTrailEmitter::SetRandom(float Min, float Max)
{
    m_RandMin = Min;
    m_RandMax = Max;
}

void CTrailEmitter::Generate(unsigned int l_NumParticles)
{
    m_Particles.resize(l_NumParticles);

    /*for(unsigned int i = 0; i < l_NumParticles; ++i)
    {
        NewParticle(&m_Particles[i]);
    }*/
}

void CTrailEmitter::NewParticle(CParticle *l_Particle)
{
    float l_Pitch = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxPitch));
    float l_Yaw = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxYaw));
    float l_LifeTime = baseUtils::RandRange(m_MinLifetime, m_MaxLifetime);
    
    float x =  Math::Utils::Cos(l_Yaw) *  Math::Utils::Cos(l_Yaw + Math::pi32 * 0.5f);
    float y =  Math::Utils::Sin(l_Pitch);
    float z =  Math::Utils::Sin(l_Yaw) + Math::Utils::Sin(l_Yaw + Math::pi32 * 0.5f);
    
    Math::Vect3f l_Vector = Math::Vect3f(x, y, z).Normalize();
    l_Vector.x *= m_Velocity.x;
    l_Vector.y *= m_Velocity.y;
    l_Vector.z *= m_Velocity.z;
    
    l_Particle->SetVelocity(l_Vector);
    l_Particle->SetAcceleration(m_Acceleration);
    
    l_Particle->SetTextureName(m_TextureName);
    l_Particle->SetSize(m_SizeX, m_SizeY);
    
    l_Particle->SetPosition(m_Position);
    l_Particle->SetLifeTime(l_LifeTime);
    l_Particle->SetIsAlive(true);
}

void CTrailEmitter::Update(float dt)
{
    if(m_Particles.size() == 0) return;
    if(!m_Active) return;

    m_EmitterLifeTime -= dt;
    if(m_EmitterLifeTime < 0.0f) 
		m_Active = false;

    int lNumParticles = m_Particles.size();
    CParticle *p = &m_Particles[0];

    omp_set_num_threads(2);
    #pragma omp parallel for
    for(int i = 0; i < lNumParticles; ++i)
    {
        (p + i)->Update(dt);
    }

	m_ActualTime += dt;
	if(m_ActualTime >= m_TimeToEmit)
    {
        m_Rand = baseUtils::RandRange(m_RandMin, m_RandMax);
        m_ActualTime = 0.0f;
    }

    CParticle *l_Particle = &m_Particles[0];
    int l_NewRand = (int) std::ceilf(m_Rand * dt);

    for(int i = 0; i < lNumParticles && l_NewRand > 0; ++i)
    {
        if((l_Particle + i)->GetIsAlive()) continue;

        NewParticle(l_Particle + i);
        l_NewRand--; m_Rand--;
    }
}

void CTrailEmitter::Render()
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->Render();
    }
}