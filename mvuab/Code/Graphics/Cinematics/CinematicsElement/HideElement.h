#ifndef _HIDE_ELEMENT_
#define _HIDE_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CHideElement : public CCinematicsElement
{
    protected:
     
    public:
        CHideElement(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
		virtual void Render(CGraphicsManager &GM);
		virtual void Render2D(CGraphicsManager &GM);
};

#endif //_HIDE_ELEMENT_