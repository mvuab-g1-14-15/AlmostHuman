#ifndef _TRIGGER_H
#define _TRIGGER_H

#include "Utils\Name.h"
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Utils\Defines.h"

class CXMLTreeNode;
class CPhysicUserData;
class CPhysicActor;
class CShape;
class CEffectTechnique;

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
  CTrigger( std::string name, Math::Vect3f position, Math::Vect3f size, Math::CColor color, int group, bool paint, bool bEnter, bool bStay, bool bLeave, std::string enterScript, std::string stayScript, std::string leaveScript );
  virtual ~CTrigger();
  void Release();
  void Destroy();

  void Render();

  std::string GetLUAByName( unsigned int Type );
  void SetLUAByName( unsigned int Type, std::string aScript );

  GET_SET( bool, bEnter );
  GET_SET( bool, bStay );
  GET_SET( bool, bLeave );

  GET_SET(Math::Vect3f, Size);

  void SetPaint( bool aPaint );
  bool GetPaint();

  void SetPosition( Math::Vect3f aPos );
  Math::Vect3f GetPosition();

  void SetActive( bool aActive )
  {
	  mActive = aActive;
  };
  bool GetActive() { return mActive; };

private: // Members

	bool mActive;

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

  CPhysicUserData*  m_PhysicUserData;
  CPhysicActor*     m_PhysicActor;
  CShape*       mShape;
  CEffectTechnique* mTechnique;

private: // Methods
};

#endif //_TRIGGER_H
