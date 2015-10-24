#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "Object3D.h"

class CParticleInstance : public CObject3D
{
public:
  CParticleInstance( const CXMLTreeNode& atts );
  CParticleInstance( const Math::Vect3f& aPosition );
  ~CParticleInstance();
};

#endif //_INSTANCE_PARTICLE_H