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
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:
		    CCinematic m_Cinematic;
        bool       m_Playing;
};

#endif //_PLAY_ANIMATION_ELEMENT_