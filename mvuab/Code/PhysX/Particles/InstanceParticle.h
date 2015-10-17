#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "Object3D.h"

class CParticleInstance : public CObject3D
{
private:
    bool mActive;
    float lCurrentTime;

public:
  CParticleInstance( const CXMLTreeNode& atts );
  CParticleInstance( const Math::Vect3f& aPosition );
  ~CParticleInstance();
  void Update( float dt ) { lCurrentTime += dt;};
  bool IsActive() const { return mActive; }
  void Activate( bool aActivate ) { mActive = aActivate; }
};

#endif //_INSTANCE_PARTICLE_H