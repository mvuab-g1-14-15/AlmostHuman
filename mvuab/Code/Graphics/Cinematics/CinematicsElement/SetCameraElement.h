#ifndef _SET_CAMERA_ELEMENT_
#define _SET_CAMERA_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CCamera;
class CGraphicsManager;

class CSetCameraElement : public CCinematicsElement
{
    public:
      CSetCameraElement(CXMLTreeNode& atts);
      virtual void Update();
      virtual void Execute(CGraphicsManager& GM);
	  virtual void Render();
	  virtual void Render2D(CGraphicsManager &GM);
    private:
	  CCamera* m_CameraCinematical;
	  std::string		m_CameraName;
    std::string		m_CurrentCameraName;
};

#endif //_SET_CAMERA_ELEMENT_