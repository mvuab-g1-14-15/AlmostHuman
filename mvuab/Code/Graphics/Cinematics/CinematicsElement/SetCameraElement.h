#ifndef _SET_CAMERA_ELEMENT_
#define _SET_CAMERA_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetCameraElement : public CCinematicsElement
{
    public:
      CSetCameraElement(CXMLTreeNode& atts);
      virtual void Execute(CGraphicsManager& GM);
		  virtual void Render(CGraphicsManager &GM);
		  virtual void Render2D(CGraphicsManager &GM);
    private:
		  Math::Vect3f		m_Position;
      float           m_Yaw;
      float           m_Pitch;
      float           m_Roll;
};

#endif //_SET_CAMERA_ELEMENT_