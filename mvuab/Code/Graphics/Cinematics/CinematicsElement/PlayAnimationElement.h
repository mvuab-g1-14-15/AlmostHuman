#ifndef _PLAY_ANIMATION_ELEMENT_
#define _PLAY_ANIMATION_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "Cinematics\Cinematic.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CPlayAnimationElement : public CCinematicsElement
{
    public:
        CPlayAnimationElement(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
		virtual void Render(CGraphicsManager &GM);
		virtual void Render2D(CGraphicsManager &GM);
	private:
		CCinematic m_Cinematic;
};

#endif //_PLAY_ANIMATION_ELEMENT_