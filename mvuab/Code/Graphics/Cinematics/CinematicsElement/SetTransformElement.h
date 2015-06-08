#ifndef _SET_TRANSFORM_ELEMENT_
#define _SET_TRANSFORM_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CInstanceMesh;
class CPhysicActor;

class CSetTransformElement: public CCinematicsElement
{
    public:
      CSetTransformElement(CXMLTreeNode& atts);
      virtual void Update();
      virtual void Execute(CGraphicsManager& GM);
		  virtual void Render();
		  virtual void Render2D(CGraphicsManager &GM);
    
  private:
      CInstanceMesh*	  m_pObject;
	  CPhysicActor*	  m_pActorObject;
  	  Math::Vect3f        m_Position;
  	  Math::Vect3f        m_Size;
      float               m_Yaw;
      float               m_Pitch;
      float               m_Roll;
};

#endif //_SET_TRANSFORM_ELEMENT_