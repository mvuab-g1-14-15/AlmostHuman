#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "Object3D.h"

class CParticleSystemCore;

class CInstanceParticle : public CObject3D
{
private:
	CParticleSystemCore* mCore;
public:
  CInstanceParticle( const CXMLTreeNode& atts );
  ~CInstanceParticle();

  void Update(){};
  void Render();
};

#endif //_INSTANCE_PARTICLE_H