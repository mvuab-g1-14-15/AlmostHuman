#include "Cinematics\CinematicsElement\RemovePhysxElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"

CRemovePhysxElement::CRemovePhysxElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Actor( PhysXMInstance->CMapManager<CPhysicActor>::GetResource( atts.GetAttribute<std::string>("resource", "") ))
{	
}

void CRemovePhysxElement::Update()
{
	PhysXMInstance->ReleasePhysicActor(m_Actor);
	m_CurrentTime += constFrameTime;
}

void CRemovePhysxElement::Execute( CGraphicsManager& GM )
{

}

void CRemovePhysxElement::Render()
{
}

void CRemovePhysxElement::Render2D(CGraphicsManager &GM)
{
}