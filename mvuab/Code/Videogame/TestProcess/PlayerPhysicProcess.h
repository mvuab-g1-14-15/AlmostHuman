#ifndef _PLAYER_PHYSIC_PROCESS_H
#define _PLAYER_PHYSIC_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

class CPhysicActor;
class CPhysicUserData;
class CPhysicController;
class CCharacter;
class CGrenade;

class CPlayerPhysicProcess : public CProcess
{
private:
  std::vector<CPhysicActor*>    m_vPA;
  std::vector<CPhysicUserData*> m_vPUD;
  std::vector<CCharacter*>      m_vCharacter;
  CPhysicController*            m_PhysicController;
  CGrenade*                     m_Grenade;
  //CCharacter*               m_Character;

public:
  CPlayerPhysicProcess();
  virtual ~CPlayerPhysicProcess();

  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();
  CPhysicUserData*    GetNewPUD( const std::string& Name );
  CPhysicActor*       GetNewPhysicActor( CPhysicUserData* PUD );
  CPhysicController*  GetNewController( float _fRadius, float _fHeight, float _fSlope, float _fSkinwidth, float _fStepOffset,
                                        ECollisionGroup _uiCollisionGroups, CPhysicUserData* _pUserData, const Math::Vect3f& _vPos, float _fGravity );
  CCharacter* GetNewCharacter( const std::string& Name );
  void                AddPudVector( CPhysicUserData* PUD );
  void                AddPhysicActorVector( CPhysicActor* PA );
  CPhysicUserData*    GetLastPUDInserted();

  void InitSceneCharacterController();


};

#endif // _PLAYER_PHYSIC_PROCESS_H
