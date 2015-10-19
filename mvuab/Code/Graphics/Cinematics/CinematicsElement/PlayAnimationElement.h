#ifndef _PLAY_ANIMATION_ELEMENT_
#define _PLAY_ANIMATION_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"

#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CCinematic;
class CAnimatedInstanceModel;

class CPlayAnimationElement : public CCinematicsElement
{
    public:
        CPlayAnimationElement(CXMLTreeNode& atts);
		~CPlayAnimationElement();
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:
		    CCinematic* m_Cinematic;
			CAnimatedInstanceModel* m_pModeloAnimado;
			bool m_Animated;
			bool       m_Playing;
			std::string m_Animation;
};

#endif //_PLAY_ANIMATION_ELEMENT_