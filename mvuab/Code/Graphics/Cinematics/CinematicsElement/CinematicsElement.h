#ifndef CINEMATICS_ELEMENT_
#define CINEMATICS_ELEMENT_
#pragma once


#include "Utils\Name.h"
#include "Utils\Visible.h"
#include "XML\XMLTreeNode.h"
#include "Utils/Types.h"

class CGraphicsManager;

class CCinematicsElement: public CVisible, public CName
{
    public:
        CCinematicsElement(CXMLTreeNode &atts);
        virtual ~CCinematicsElement();

        virtual void Execute(CGraphicsManager &GM) = 0;
		virtual void Render(CGraphicsManager &GM) = 0;
		virtual void Render2D(CGraphicsManager &GM) = 0;
private:
	bool mIsBlocker;
	float32 mTime;
};
#endif //CINEMATICS_ELEMENT_