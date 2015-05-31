#ifndef _SET_CAMERA_ELEMENT_
#define _SET_CAMERA_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetCameraElement : public CCinematicsElement
{
    private:
		std::string			m_sCamera;
		Math::Vect3f		m_Position;
    public:
        CSetCameraElement(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
		virtual void Render(CGraphicsManager &GM);
		virtual void Render2D(CGraphicsManager &GM);
};

#endif //_SET_CAMERA_ELEMENT_