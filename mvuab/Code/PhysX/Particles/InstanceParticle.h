#ifndef _INSTANCE_PARTICLE_H
#define _INSTANCE_PARTICLE_H

#include "XML\XMLTreeNode.h"
#include "Object3D.h"
#include "Utils/Name.h"

class CParticleSystemCore;

class CParticleInstance : public CObject3D, public CName
{
public:
  CParticleInstance( const CXMLTreeNode& atts );
  CParticleInstance( const std::string& aName, const std::string& aCoreName, const std::string& aRoomName );
  ~CParticleInstance();
  void Update( float dt );
  void Render();
  bool IsOk() const { return mIsOk; }
  void SetRoomName( const std::string& aRoomName ) { mRoomName = aRoomName; }
  void Reset();
  void SetDirection( const Math::Vect3f& aDirection );
private:
  CParticleSystemCore* mCore;
  std::string          mRoomName;
  bool                 mIsOk;
};

#endif //_INSTANCE_PARTICLE_H