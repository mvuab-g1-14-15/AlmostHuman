#ifndef Blaster_H
#define Blaster_H

#include "Math/Vector3.h"

#include <string>
#include <vector>
class CPhysicActor;
class CPhysicUserData;

class CBlaster
{
private:
  float m_TimeAux;
  float m_PressedTime;
  float m_Force;
  std::vector<CPhysicUserData*> m_vPhysicUserData;
  CPhysicUserData* m_PhysicUserData;
  std::vector<CPhysicActor*> m_vActor;
  CPhysicActor* m_Actor;

  std::string m_MeshName;
  bool  m_BlasterShoot;
  float m_ElapsedTime;

  Math::Vect3f m_FinalPos;
  Math::Vect3f m_InitialPos;

public:
  CBlaster( float a_TimeAux, float m_PressedTime, float a_Force, std::string a_MeshName );
  virtual ~CBlaster();

  void Start();
  void Update();
  void Render();
};

#endif // Grenade_H
