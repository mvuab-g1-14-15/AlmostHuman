#ifndef _LOAD_ASE_ELEMENT_
#define _LOAD_ASE_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "Actor\PhysicActor.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CLoadASEElement : public CCinematicsElement
{
    public:
        CLoadASEElement(CXMLTreeNode& atts);
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:
		CPhysicActor* m_Actor;

};

#endif //_LOAD_ASE_ELEMENT_