#ifndef _HIDE_ELEMENT_
#define _HIDE_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CHideElement : public CCinematicsElement
{
    public:
        CHideElement(CXMLTreeNode& atts);
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
        virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
  private:
        std::string m_Type;
};

#endif //_HIDE_ELEMENT_