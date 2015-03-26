#include "CubeEmitter.h"
#include "Utils/BaseUtils.h"

#include <omp.h>

CCubeEmitter::CCubeEmitter()
{
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

void CCubeEmitter::SetRadius(float min, float max)
{
    m_MinimumRadius = min;
    m_MaximumRadius = max;
}

void CCubeEmitter::Generate (unsigned int l_NumParticles)
{
    m_Particles.resize(l_NumParticles);

    for(unsigned int i = 0; i < l_NumParticles; ++i)
    {
        NewParticleSphere(&m_Particles[i]);
    }
}

void CCubeEmitter::NewParticleSphere(CParticle *l_Particle)
{
    float l_Radius = baseUtils::RandRange(m_MinimumRadius, m_MaximumRadius);
    float l_LifeTime = baseUtils::RandRange(m_MinLifetime, m_MaxLifetime);

    float x = baseUtils::RandRange(m_Position.x - m_MinWidth  * 0.5f, m_Position.x + m_MaxWidth  * 0.5f);
    float y = baseUtils::RandRange(m_Position.y - m_MinHeight * 0.5f, m_Position.y + m_MaxHeight * 0.5f);
    float z = baseUtils::RandRange(m_Position.z - m_MinDepth  * 0.5f, m_Position.z + m_MaxDepth  * 0.5f);

    Math::Vect3f l_Vector = Math::Vect3f(x, y, z);
    l_Particle->SetPosition(l_Vector);
    

    l_Particle->SetVelocity(m_Velocity);
    l_Particle->SetAcceleration(m_Acceleration);

    l_Particle->SetTextureName(m_TextureName);
    l_Particle->SetSize(l_Radius);


    l_Particle->SetLifeTime(l_LifeTime);
    l_Particle->SetIsAlive(true);
}

void CCubeEmitter::Update(float dt)
{
    if(m_Particles.size() == 0) return;

    CParticle *p = &m_Particles[0];
    omp_set_num_threads(2);

    #pragma omp parallel for
    for(int i = 0; i < m_Particles.size(); i++)
    {
        (p + i)->Update(dt);
    }

    m_ActualTime += dt;
    if ( m_ActualTime < 1.0f )
    {
        m_Rand = rand() % ( m_Max - m_Min + 1 ) + m_Min;
        m_ActualTime = 0.0f;
    }

    for(m_Rand; m_Rand > 0; --m_Rand)
    {
        std::vector<CParticle>::iterator it = m_Particles.begin();

        for ( ; it != m_Particles.end() && it->GetIsAlive(); ++it );
        if ( it == m_Particles.end() ) return;

        NewParticleSphere( it._Ptr );
    }
}


void CCubeEmitter::Render()
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->Render();
    }
}