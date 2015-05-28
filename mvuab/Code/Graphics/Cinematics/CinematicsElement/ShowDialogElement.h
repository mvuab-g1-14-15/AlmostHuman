#ifndef _SHOW_DIALOG_ELEMENT_
#define _SHOW_DIALOG_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CShowDialogElement : public CCinematicsElement
{
    private:
		std::string		m_Text;
    public:
        CShowDialogElement(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
		virtual void Render(CGraphicsManager &GM);
		virtual void Render2D(CGraphicsManager &GM);
};

#endif //_SHOW_DIALOG_ELEMENT_