#ifndef _PHYSIC_PROCESS_H
#define _PHYSIC_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "Reports\PhysicTriggerReport.h"

#include <vector>

class CPhysicActor;
class CPhysicUserData;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CGrenade;

class CPhysicProcess : public CProcess, public CPhysicTriggerReport
{
private:
  std::vector<CPhysicActor*>      m_vPA;
  std::vector<CPhysicUserData*>   m_vPUD;
  CPhysicSphericalJoint*          m_PSJ;
  CPhysicRevoluteJoint*           m_PRJ;
  bool                            m_Salir;
  float                           m_Time;
  CGrenade*                       m_Grenade;

public:
  CPhysicProcess();
  virtual ~CPhysicProcess();

  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();
  GET_SET( bool, Salir );
  GET_SET( float, Time );
  CPhysicUserData*    GetNewPUD( const std::string& Name );
  CPhysicActor*       GetNewPhysicActor( CPhysicUserData* PUD );
  void                AddPudVector( CPhysicUserData* PUD );
  void                AddPhysicActorVector( CPhysicActor* PA );
  CPhysicUserData*    GetLastPUDInserted();
  /*GET_SET_PTR( CPhysicActor, PhysicActor );
  GET_SET_PTR( CPhysicUserData, PUD );*/
  void    OnEnter( CPhysicUserData* _Entity_Trigger1,
                   CPhysicUserData* _Other_Shape );
  void    OnLeave( CPhysicUserData* _Entity_Trigger1,
                   CPhysicUserData* _Other_Shape );
  void    OnStay( CPhysicUserData* _Entity_Trigger1,
                  CPhysicUserData* _Other_Shape );

  void InitScenePhysicsSamplers();
};

#endif // PHYSIC_PROCESS_H
