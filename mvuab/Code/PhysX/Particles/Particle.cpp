#include "Particle.h"
#include <Windows.h>

CParticle::CParticle()
{
    m_Size = 1.0f;
    m_TimeToLive = 0.0;

    m_IsAlive = false;
    m_Billboard.Init(Math::Vect3f(0.0f, 0.0f, 0.0f), 1, "");

    m_Color = Math::Vect3f(1.0f, 1.0f, 1.0f);
    m_Position = Math::Vect3f(0.0f, 0.0f, 0.0f);

    m_Velocity = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Aceleration = Math::Vect3f(0.0f, 0.0f, 0.0f);
}

CParticle::CParticle(float sz, float timer, const Math::Vect3f &Color, const Math::Vect3f &Position, const Math::Vect3f &Velocity, const Math::Vect3f &Aceleration)
{
    m_Size = sz;
    m_TimeToLive = timer;

    m_IsAlive = false;

    m_Color = Color;
    m_Position = Position;

    m_Velocity = Velocity;
    m_Aceleration = Aceleration;
}

void CParticle::SetColor(const Math::Vect3f &Color)
{
    m_Color = Color;
}

void CParticle::SetPosition(const Math::Vect3f &Position)
{
    m_Position = Position;
}

void CParticle::SetVelocity(const Math::Vect3f &Velocity)
{
    m_Velocity = Velocity;
}

void CParticle::SetAceleration(const Math::Vect3f &Aceleration)
{
    m_Aceleration = Aceleration;
} 

void CParticle::SetTimeToLive(float Time)
{
    LARGE_INTEGER l_Freq = { 0 }, l_ActualTime = { 0 };
    QueryPerformanceCounter(&l_ActualTime);  QueryPerformanceFrequency(&l_Freq);

    m_TimeToLive = Time + l_ActualTime.QuadPart / l_Freq.QuadPart * 1000.0f;
}

void CParticle::SetIsAlive(bool isAlive)
{
    m_IsAlive = isAlive;
}

const Math::Vect3f &CParticle::GetColor()
{
    return m_Color;
}

const Math::Vect3f &CParticle::GetPostion()
{
    return m_Position;
}

const Math::Vect3f &CParticle::GetVelocity()
{
    return m_Velocity;
}

const Math::Vect3f &CParticle::GetAceleration()
{
    return m_Aceleration;
}

float CParticle::GetTimeToLive()
{
    return m_TimeToLive;
}

bool CParticle::GetIsAlive()
{
    return m_IsAlive;
}

void CParticle::Update(float dt)
{
    if(!m_IsAlive) return;

    LARGE_INTEGER l_Freq = { 0 }, l_ActualTime = { 0 };
    QueryPerformanceCounter(&l_ActualTime);  QueryPerformanceFrequency(&l_Freq);

    float l_Time = l_ActualTime.QuadPart / l_Freq.QuadPart * 1000.0f;
    m_IsAlive = (m_TimeToLive - l_Time > 0.0f) ? true : false;

    Math::Vect3f l_OldVel = m_Velocity;
    
    m_Velocity += m_Aceleration * dt;
    m_Position += (m_Velocity + l_OldVel) / 2.0f * dt;

    m_Billboard.Init(m_Position, 1, "");
    m_Billboard.Update();
}

void CParticle::Render()
{
    m_Billboard.Render();
}
