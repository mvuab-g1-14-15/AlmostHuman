#include "Particle.h"
#include <Windows.h>

#include "EngineManagers.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

CParticle::CParticle()
{
    m_LifeTime = 0.0;
    m_SphereRadius = 0.0f;

    m_Billboard.SetPosition( Math::Vect3f( 0.0f, 0.0f, 0.0f ) );
    m_Billboard.SetSize(0.1f, 0.1f);

    m_IsAlive = false;

    m_Color = Math::Vect3f( 1.0f, 1.0f, 1.0f );
    m_Position = Math::Vect3f( 0.0f, 0.0f, 0.0f );

    m_Velocity = Math::Vect3f( 0.0f, 0.0f, 0.0f );
    m_Acceleration = Math::Vect3f( 0.0f, 0.0f, 0.0f );
}

CParticle::CParticle( float sz, float timer, const Math::Vect3f& Color, const Math::Vect3f& Position, const Math::Vect3f& Velocity,  const Math::Vect3f& Aceleration )
{
    m_SphereRadius = 0.0f;
    m_LifeTime = timer;
    m_IsAlive = false;

    m_Color = Color;
    m_Position = Position;

    m_Velocity = Velocity;
    m_Acceleration = Aceleration;
}

void CParticle::SetColor( const Math::Vect3f& Color )
{
    m_Color = Color;
}

void CParticle::SetPosition( const Math::Vect3f& Position )
{
    m_Position = Position;
}

void CParticle::SetVelocity( const Math::Vect3f& Velocity )
{
    m_Velocity = Velocity;
}

void CParticle::SetAcceleration( const Math::Vect3f& Acceleration )
{
    m_Acceleration = Acceleration;
}

void CParticle::SetLifeTime( float Time )
{
    LARGE_INTEGER l_Freq = { 0 }, l_ActualTime = { 0 };

    QueryPerformanceCounter(&l_ActualTime);
    QueryPerformanceFrequency(&l_Freq);

    m_LifeTime = Time + l_ActualTime.QuadPart / l_Freq.QuadPart;
}

void CParticle::SetSize( float sx, float sy)
{
    m_SphereRadius = ((sx > sy) ? sx : sy) * 0.5f;
    m_Billboard.SetSize(sx, sy);
}

void CParticle::SetIsAlive( bool isAlive )
{
    m_IsAlive = isAlive;
}

void CParticle::SetTextureName( std::string TextureName )
{
    m_Billboard.SetTexture(TextureName);
}

const Math::Vect3f& CParticle::GetColor()
{
    return m_Color;
}

const Math::Vect3f& CParticle::GetPosition()
{
    return m_Position;
}

const Math::Vect3f& CParticle::GetVelocity()
{
    return m_Velocity;
}

const Math::Vect3f& CParticle::GetAcceleration()
{
    return m_Acceleration;
}

float CParticle::GetLifeTime()
{
    return m_LifeTime;
}

bool CParticle::GetIsAlive()
{
    return m_IsAlive;
}

void CParticle::Update( float dt )
{
    if ( !m_IsAlive ) { return; }

    LARGE_INTEGER l_Freq = { 0 }, l_ActualTime = { 0 };
    QueryPerformanceCounter( &l_ActualTime );
    QueryPerformanceFrequency( &l_Freq );

    float l_Time = (float) l_ActualTime.QuadPart / (float) l_Freq.QuadPart;
    m_IsAlive = ( m_LifeTime - l_Time > 0.0f ) ? true : false;

    Math::Vect3f l_OldVel = m_Velocity;

    m_Velocity += m_Acceleration * dt;
    m_Position += ( ( m_Velocity + l_OldVel ) * 0.5f ) * dt;

    m_Billboard.SetPosition( m_Position );
    m_Billboard.Update();
}

void CParticle::Render()
{
    if(!m_IsAlive) { return; }

    CCameraManager *l_CM = CameraMInstance;
    D3DXVECTOR3 l_Pos = D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z);

    if(l_CM->GetCurrentCamera()->GetFrustum().SphereVisible(l_Pos, m_SphereRadius))
    { m_Billboard.Render(); }
}