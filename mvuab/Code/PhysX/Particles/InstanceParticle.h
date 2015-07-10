#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "ParticleSystemCore.h"

#include <string>
#include <vector>

class CInstanceParticle
{
private:
  CParticleSystemCore*  m_ParticleSystem;
  Math::Vect3f          m_Position;
public:
  CInstanceParticle();
  CInstanceParticle( const CXMLTreeNode& atts );
  ~CInstanceParticle();

  void Update();
  void Render();
};

#endif //_INSTANCE_PARTICLE_H