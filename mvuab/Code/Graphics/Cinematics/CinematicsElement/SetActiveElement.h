#ifndef _SET_ACTIVE_ELEMENT_
#define _SET_ACTIVE_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "RenderableObject\RenderableObject.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetActiveElement : public CCinematicsElement
{
    public:
        CSetActiveElement(CXMLTreeNode& atts);
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:
		    std::vector<CRenderableObject*> m_vRenderableObject;
        bool       m_bActive;
};

#endif //_SET_ACTIVE_ELEMENT_