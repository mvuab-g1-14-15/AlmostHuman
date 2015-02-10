#include "Trigger.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "PhysicsManager.h"
#include "Utils\Defines.h"

CTrigger::CTrigger(const CXMLTreeNode& Node)
  : CName( Node.GetPszProperty("name", "unknown") )
  , m_Position( Node.GetVect3fProperty("position", Math::Vect3f(0,0,0)) )
  , m_Size( Node.GetVect3fProperty("size", Math::Vect3f(0,0,0)) )
  , m_Color( Node.GetCColorProperty("size", Math::CColor(0,0,0,0)))
  , m_Group( Node.GetIntProperty("group", 0) )
  , m_Paint( Node.GetBoolProperty("paint", false ) )
  , m_PhysicUserData( new CPhysicUserData(Node.GetPszProperty("name", "unknown")) )
{
  if( Node.GetBoolProperty("enter_event", false ) )
    m_Enter= ( std::make_pair( ENTER, Node.GetPszProperty("enter_script", "unknown") ) );

  if( Node.GetBoolProperty("leave_event", false ) )
    m_Leave = ( std::make_pair( LEAVE, Node.GetPszProperty("leave_script", "unknown") ) );

  if( Node.GetBoolProperty("stay_event", false ) )
    m_Stay = ( std::make_pair( STAY, Node.GetPszProperty("stay_script", "unknown") ) );

  m_PhysicUserData->SetPaint(m_Paint);
  m_PhysicUserData->SetColor(m_Color);
  m_PhysicUserData->SetGroup(ECG_TRIGGERS);
  m_PhysicActor = new CPhysicActor(m_PhysicUserData);

  const std::string & l_sType = Node.GetPszProperty("shape","");
  if(l_sType=="Box")
  {
    m_PhysicActor->CreateBoxTrigger(m_Position,m_Size,m_Group);
  }
  else if(l_sType=="Sphere")
  {
    m_PhysicActor->CreateSphereTrigger(m_Position,m_Radius,m_Group);
  }
  
  if ( l_sType == "" )
  {
    CHECKED_DELETE(m_PhysicActor);
  }
}

CTrigger::~CTrigger()
{
  // TODO RAUL
  // DELETE THE ACTORS OF THE TRIGGER AND THE USER DATA
}