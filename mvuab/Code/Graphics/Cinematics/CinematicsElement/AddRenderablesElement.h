#ifndef _ADD_RENDERABLE_ELEMENT_
#define _ADD_RENDERABLE_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CAddRenderablesElement : public CCinematicsElement
{
    public:
        CAddRenderablesElement(CXMLTreeNode& atts);
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:

};

#endif //_SET_ACTIVE_ELEMENT_