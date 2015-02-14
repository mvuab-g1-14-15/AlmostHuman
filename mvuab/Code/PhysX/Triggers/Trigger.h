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
  CTrigger( const CXMLTreeNode& Node );
  virtual ~CTrigger();
  void Release();
  void Destroy();

private: // Members
  enum TEventType
  {
    ENTER = 0,
    LEAVE,
    STAY
  };

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

  CPhysicUserData* m_PhysicUserData;
  CPhysicActor*    m_PhysicActor;

private: // Methods
};

#endif //_TRIGGER_H
