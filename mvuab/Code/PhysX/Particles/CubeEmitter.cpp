#include "CubeEmitter.h"
#include "Utils/BaseUtils.h"

#include <omp.h>

CCubeEmitter::CCubeEmitter()
{
    float m_MinWidth = 0.0f;
    float m_MaxWidth = 0.0f;

    float m_MinHeight = 0.0f;
    float m_MaxHeight = 0.0f;

    float m_MinDepth = 0.0f;
    float m_MaxDepth = 0.0f;
}

CCubeEmitter::~CCubeEmitter()
{
}

void CCubeEmitter::SetDepth(float min, float max)
{
    m_MinDepth = min;
    m_MaxDepth = max;
}

void CCubeEmitter::SetWidth(float min, float max)
{
    m_MinWidth = min;
    m_MaxWidth = max;
}

void CCubeEmitter::SetHeight(float min, float max)
{
    m_MinHeight = min;
    m_MaxHeight = max;
}

void CCubeEmitter::SetRandom(float Min, float Max)
{
    m_RandMin = Min;
    m_RandMax = Max;
}

void CCubeEmitter::Generate(unsigned int l_NumParticles, bool l_Generate)
{
    m_Particles.resize(l_NumParticles);

    for(unsigned int i = 0; i < l_NumParticles && l_Generate; ++i)
    {
        NewParticle(&m_Particles[i]);
    }
}

void CCubeEmitter::NewParticle(CParticle *l_Particle)
{
    float l_LifeTime = baseUtils::RandRange(m_MinLifetime, m_MaxLifetime);

    float x = baseUtils::RandRange(m_Position.x - m_MinWidth  * 0.5f, m_Position.x + m_MaxWidth  * 0.5f);
    float y = baseUtils::RandRange(m_Position.y - m_MinHeight * 0.5f, m_Position.y + m_MaxHeight * 0.5f);
    float z = baseUtils::RandRange(m_Position.z - m_MinDepth  * 0.5f, m_Position.z + m_MaxDepth  * 0.5f);

    Math::Vect3f l_Vector = Math::Vect3f(x, y, z);
    l_Particle->SetPosition(l_Vector);
    
    l_Particle->SetVelocity(m_Velocity);
    l_Particle->SetAcceleration(m_Acceleration);

    l_Particle->SetTextureName(m_TextureName);
    l_Particle->SetSize(m_SizeX, m_SizeY);

    l_Particle->SetLifeTime(l_LifeTime);
    l_Particle->SetIsAlive(true);
}

void CCubeEmitter::Update(float dt)
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

void CCubeEmitter::Render()
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->Render();
    }
}