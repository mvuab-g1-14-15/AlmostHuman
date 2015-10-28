#ifndef _REMOVE_PHYSX_ELEMENT_
#define _REMOVE_PHYSX_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "Actor\PhysicActor.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRemovePhysxElement : public CCinematicsElement
{
    public:
        CRemovePhysxElement(CXMLTreeNode& atts);
        virtual void Update();
        virtual void Execute(CGraphicsManager& GM);
		    virtual void Render();
		    virtual void Render2D(CGraphicsManager &GM);
	private:
		CPhysicActor* m_Actor;

};

#endif //_REMOVE_PHYSX_ELEMENT_