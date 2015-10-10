#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "Object3D.h"

class CParticleInstance : public CObject3D
{
private:
    bool mActive;

public:
  CParticleInstance( const CXMLTreeNode& atts );
  ~CParticleInstance();
  bool IsActive() const { return mActive; }
};

#endif //_INSTANCE_PARTICLE_H