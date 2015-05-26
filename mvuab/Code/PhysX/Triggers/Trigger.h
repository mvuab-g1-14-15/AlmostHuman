#ifndef _TRIGGER_H
#define _TRIGGER_H

#include "Utils\Name.h"
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Utils\Defines.h"

class CXMLTreeNode;
class CPhysicUserData;
class CPhysicActor;

class CTrigger : public CName
{
public:
  enum TEventType
  {
    ENTER = 0,
    LEAVE,
    STAY
  };

  CTrigger( const CXMLTreeNode& Node );
  virtual ~CTrigger();
  void Release();
  void Destroy();
  std::string GetLUAByName( unsigned int Type );
  GET_SET_PTR(CPhysicActor, PhysicActor);
  GET_SET(bool, bEnter);
  GET_SET(bool, bStay);
  GET_SET(bool, bLeave);	

private: // Members


  Math::Vect3f m_Position;
  Math::Vect3f m_Size;
  float32      m_Radius;
  Math::CColor m_Color;
  int32        m_Group;

  // The enter events
  typedef std::pair<TEventType, std::string> TTriggerEvent;
  TTriggerEvent m_Enter;
  TTriggerEvent m_Stay;
  TTriggerEvent m_Leave;

  bool m_Paint;
  bool m_bEnter;
  bool m_bStay;
  bool m_bLeave;

  CPhysicUserData* m_pPhysicUserData;
  CPhysicActor*    m_pPhysicActor;

private: // Methods
};

#endif //_TRIGGER_H
