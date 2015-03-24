#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"

#include "Core.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

#include <omp.h>

CParticleEmitter::CParticleEmitter()
{
  m_Particles.resize( 0 );
  m_TimeToLive = 0.0f;

  m_ActualTime = 0.0f;
  m_PrevTime = 0.0f;

  m_Min = 0;
  m_Max = 0;

  m_Position = Math::Vect3f( 0.0f, 0.0f, 0.0f );
  m_Direction = Math::Vect3f( 0.0f, 0.0f, 0.0f );
  m_Velocity = Math::Vect3f( 0.0f, 0.0f, 0.0f );
}

CParticleEmitter::~CParticleEmitter()
{
  m_Particles.clear();
}

void CParticleEmitter::Update( float dt )
{
  CParticle* p = &m_Particles[0];
  omp_set_num_threads( 2 );

  #pragma omp parallel for

  for ( int i = 0; i < m_Particles.size(); i++ )
    ( p + i )->Update( dt );


  m_ActualTime += dt;

  if ( m_ActualTime >= 1.0f )
  {
    m_Rand = rand() % ( m_Max - m_Min + 1 ) + m_Min;
    m_PrevTime = m_ActualTime;
    m_ActualTime = 0.0f;
  }

  if ( m_PrevTime > 0.0f )
  {
    m_PrevTime -= dt;
    std::vector<CParticle>::iterator it = m_Particles.begin();

    for ( ; it != m_Particles.end() && it->GetIsAlive(); ++it );

    if ( it == m_Particles.end() ) return;

    NewParticle( it );
  }

  /*if(m_ActualTime >= 1.0f)
  {
      unsigned int l_Rand = rand() % (m_Max - m_Min + 1) + m_Min;
      while(l_Rand-- > 0)
      {
          std::vector<CParticle>::iterator it = m_Particles.begin();
          for(; it != m_Particles.end() && it->GetIsAlive(); ++it);
          if(it == m_Particles.end()) return;

          NewParticle(it);
      }

      m_ActualTime = 0.0f;
  }*/
  /*
  unsigned int l_NumParticles = GetNumActiveParticles();
  while(l_NumParticles < (m_Max - m_Min))
  {
      std::vector<CParticle>::iterator it = m_Particles.begin();
      for(; it != m_Particles.end() && it->GetIsAlive(); ++it);
      if(it == m_Particles.end()) return;

      NewParticle(it);
      l_NumParticles++;
  }*/
}

void CParticleEmitter::Render()
{
  CCameraManager* l_CM = CCore::GetSingletonPtr()->GetCameraManager();

  for ( std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it )
  {
    if ( l_CM->GetCurrentCamera()->GetFrustum().BoxVisibleByVertexs( &it->GetPosition() ) )
      it->Render();
  }
}

void CParticleEmitter::SetAcceleration( const Math::Vect3f& Acceleration )
{
  m_Acceleration = Acceleration;
}

void CParticleEmitter::SetDirection( const Math::Vect3f& Direction )
{
  m_Direction = Direction;
}

void CParticleEmitter::SetPosition( const Math::Vect3f& Position )
{
  m_Position = Position;
}
void CParticleEmitter::SetVelocity( const Math::Vect3f& Velocity )
{
  m_Velocity = Velocity;
}

void CParticleEmitter::SetTimeToLive( float timeToLive )
{
  m_TimeToLive = timeToLive;
}

void CParticleEmitter::SetMin( unsigned int min )
{
  m_Min = min;
}

void CParticleEmitter::SetMax( unsigned int max )
{
  m_Max = max;
}

void CParticleEmitter::Generate( unsigned int numParticles )
{
  m_Particles.resize( numParticles );

  for ( std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it )
    NewParticle( it );
}

void CParticleEmitter::NewParticle( std::vector<CParticle>::iterator it )
{
  float l_RandX = 2.0f * ( ( float ) rand() / ( float ) RAND_MAX ) - 1.0f;
  float l_RandY = 2.0f * ( ( float ) rand() / ( float ) RAND_MAX ) - 1.0f;
  float l_RandZ = 2.0f * ( ( float ) rand() / ( float ) RAND_MAX ) - 1.0f;

  it->SetPosition( m_Position );
  it->SetAcceleration( m_Acceleration );
  it->SetVelocity( m_Velocity + Math::Vect3f( l_RandX, l_RandY, l_RandZ ) );

  it->SetTextureName( m_TextureName );

  float l_LifeTime = 3.0f * ( ( float ) rand() / ( float ) RAND_MAX );
  it->SetTimeToLive( m_TimeToLive + l_LifeTime );
  it->SetIsAlive( true );
}

unsigned int CParticleEmitter::GetNumActiveParticles()
{
  unsigned int l_Particles = 0;

  for ( std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it )
  {
    if ( it->GetIsAlive() ) ++l_Particles;
  }

  return l_Particles;
}

void CParticleEmitter::SetTextureName( std::string TextureName )
{
  m_TextureName = TextureName;
  unsigned int l_Particles = 0;

  for ( std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it )
  {
    if ( it->GetIsAlive() ) it->SetTextureName( m_TextureName );
  }
}
