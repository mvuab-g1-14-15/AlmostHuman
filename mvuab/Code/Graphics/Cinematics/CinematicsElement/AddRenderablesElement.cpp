#include "Cinematics\CinematicsElement\AddRenderablesElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "StaticMeshes\InstanceMesh.h"
#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "StaticMeshes\StaticMesh.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

CAddRenderablesElement::CAddRenderablesElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    
{
	std::string lPathBase = atts.GetAttribute<std::string>("path", "");
	std::string lPathRenderable = lPathBase + atts.GetAttribute<std::string>("renderable_objects_file", "");
	std::string lPathStaticMesh = lPathBase + atts.GetAttribute<std::string>("static_meshes_file", "");
	SMeshMInstance->Load(lPathStaticMesh, lPathBase); 
	SceneInstance->GetResource(atts.GetAttribute<std::string>("room", ""))->GetLayer("solid")->Load(lPathRenderable);
}

void CAddRenderablesElement::Update()
{
	m_CurrentTime += constFrameTime;
}

void CAddRenderablesElement::Execute( CGraphicsManager& GM )
{

}

void CAddRenderablesElement::Render()
{
}

void CAddRenderablesElement::Render2D(CGraphicsManager &GM)
{
}