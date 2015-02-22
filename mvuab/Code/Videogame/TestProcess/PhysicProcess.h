#ifndef PHYSIC_PROCESS_H
#define PHYSIC_PROCESS_H
#pragma once

#include "Process.h"
#include <vector>
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "Reports\PhysicTriggerReport.h"
#include "Utils\Defines.h"

class CPhysicActor;
class CPhysicUserData;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CTriggerManager;

class CPhysicProcess : public CProcess, public CPhysicTriggerReport
{
private:
  float32                         m_Speed;
  float32                         m_Amount;
  float32                         m_Angle;
  float32                         m_AngleMoon;
  Math::Vect2i                    m_LastMousePoint;
  bool                            m_PaintAll;
  std::vector<CPhysicActor*>     m_vPA;
  std::vector<CPhysicUserData*>  m_vPUD;
  /*CPhysicActor*                   m_pPhysicActor;
  CPhysicUserData*                m_pPUD;*/
  CPhysicSphericalJoint*          m_PSJ;
  CPhysicRevoluteJoint*           m_PRJ;
  CTriggerManager*                m_TriggerManager;
  bool                            m_Salir;
  float                            m_Time;

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


};

#endif // PHYSIC_PROCESS_H
