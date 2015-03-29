#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Math\Vector3.h"
#include "Particle.h"
#include "Utils\Defines.h"

#include <vector>

class CParticleEmitter
{
protected:
  std::vector<CParticle> m_Particles;

  unsigned int m_Rand;
  unsigned int m_Min;
  unsigned int m_Max;

  float m_MinLifetime;
  float m_MaxLifetime;
  float m_ActualTime;

  float m_Time;

  Math::Vect3f m_Acceleration;
  Math::Vect3f m_Direction;
  Math::Vect3f m_Position;
  Math::Vect3f m_Velocity;

  std::string m_TextureName;
  bool m_Active;

  float m_EmitterLifeTime;
public:
  CParticleEmitter();
  virtual ~CParticleEmitter();

  void SetAcceleration( const Math::Vect3f& Acceleration );
  void SetVelocity( const Math::Vect3f& Velocity );

  void SetDirection( const Math::Vect3f& Direction );
  void SetPosition( const Math::Vect3f& Position );

  void SetLifeTime( float min, float max );

  GET_SET( std::string, TextureName );
  GET_SET( size_t, Min );
  GET_SET( size_t, Max );
  GET_SET( bool, Active );
  GET_SET( float, EmitterLifeTime );
  GET_SET( float, Time );

  virtual void Generate( unsigned int l_NumParticles ) = 0;
  virtual void Update( float dt ) = 0;
  virtual void Render() = 0;
};

#endif
