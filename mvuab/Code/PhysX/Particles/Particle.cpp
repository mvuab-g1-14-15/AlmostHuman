#include "Particle.h"
#include <Windows.h>

CParticle::CParticle()
{
  m_Size = 1.0f;
  m_LifeTime = 0.0;

  m_IsAlive = false;
  m_Billboard.Init( Math::Vect3f( 0.0f, 0.0f, 0.0f ), 1, "" );

  m_Color = Math::Vect3f( 1.0f, 1.0f, 1.0f );
  m_Position = Math::Vect3f( 0.0f, 0.0f, 0.0f );

  m_Velocity = Math::Vect3f( 0.0f, 0.0f, 0.0f );
  m_Acceleration = Math::Vect3f( 0.0f, 0.0f, 0.0f );
}

CParticle::CParticle( float sz, float timer, const Math::Vect3f& Color, const Math::Vect3f& Position, const Math::Vect3f& Velocity,
                      const Math::Vect3f& Aceleration )
{
  m_Size = sz;
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
  QueryPerformanceCounter( &l_ActualTime );
  QueryPerformanceFrequency( &l_Freq );

  m_LifeTime = Time + l_ActualTime.QuadPart / l_Freq.QuadPart;
}

void CParticle::SetIsAlive( bool isAlive )
{
  m_IsAlive = isAlive;
}

void CParticle::SetTextureName( std::string TextureName )
{
  m_TextureName = TextureName;
  m_Billboard.SetTexture( m_TextureName );
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
  if ( !m_IsAlive ) return;

  LARGE_INTEGER l_Freq = { 0 }, l_ActualTime = { 0 };
  QueryPerformanceCounter( &l_ActualTime );
  QueryPerformanceFrequency( &l_Freq );

  float l_Time = (float) l_ActualTime.QuadPart / (float) l_Freq.QuadPart;
  m_IsAlive = ( m_LifeTime - l_Time > 0.0f ) ? true : false;

  Math::Vect3f l_OldVel = m_Velocity;

  m_Velocity += m_Acceleration * dt;
  m_Position += ( ( m_Velocity + l_OldVel ) / 2.0f ) * dt;

  m_Billboard.Init( m_Position, 0.6f, m_TextureName );
  m_Billboard.Update();
}

void CParticle::Render()
{
  if ( m_IsAlive ) m_Billboard.Render();
}
