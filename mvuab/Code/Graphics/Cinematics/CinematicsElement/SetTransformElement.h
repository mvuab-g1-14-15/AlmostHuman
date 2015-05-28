#ifndef _SET_TRANSFORM_ELEMENT_
#define _SET_TRANSFORM_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetTransformElement: public CCinematicsElement
{
    private:
		Math::Vect3f m_Position;

    public:
        CSetTransformElement(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
		virtual void Render(CGraphicsManager &GM);
		virtual void Render2D(CGraphicsManager &GM);
};

#endif //_SET_TRANSFORM_ELEMENT_