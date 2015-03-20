#include "ParticleEmitter.h"
#include <time.h>

CParticleEmitter::CParticleEmitter()
{
    srand(time(0));

    m_Particles.resize(200);
    m_TimeToLive = 0.0f;

    m_Position = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Direction = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Velocity = Math::Vect3f(0.0f, 0.0f, 0.0f);
}

CParticleEmitter::~CParticleEmitter()
{
    m_Particles.clear();
}

void CParticleEmitter::Update(float dt)
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->Update(dt);
    }
}

void CParticleEmitter::Render()
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->Render();
    }
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

void CParticleEmitter::SetTimeToLive(float timeToLive)
{
    m_TimeToLive = timeToLive;
}

void CParticleEmitter::Generate()
{
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        it->SetPosition(m_Position);
        it->SetAceleration(Math::Vect3f(0.0f, -9.87f, 0.0f));
        it->SetVelocity(m_Direction + Math::Vect3f(rand() % 5, rand() % 5, rand() % 5));

        it->SetIsAlive(true);
        it->SetTimeToLive(m_TimeToLive + (float)(rand() % 500));
    }
}

void CParticleEmitter::NewParticle()
{
    std::vector<CParticle>::iterator it = m_Particles.begin();
    for(; it != m_Particles.end() && !it->GetIsAlive(); ++it);
    if(it == m_Particles.end()) return;
    
    it->SetPosition(m_Position);
    it->SetAceleration(Math::Vect3f(0.0f, -9.87f, 0.0f));
    it->SetVelocity(m_Direction + Math::Vect3f(rand() % 5, 0.0f, rand() % 5));
    
    it->SetIsAlive(true);
    it->SetTimeToLive(m_TimeToLive + (float)(rand() % 500));
}
