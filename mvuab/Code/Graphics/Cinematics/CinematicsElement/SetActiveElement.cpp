#include "Cinematics\CinematicsElement\SetActiveElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "StaticMeshes\StaticMesh.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

CSetActiveElement::CSetActiveElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_bActive( atts.GetAttribute<bool>("active", false ) )
{
	CXMLTreeNode newFile, element;
	if ( newFile.LoadAndFindNode( atts.GetAttribute<std::string>("resource_id_file", "").c_str(), "elements" , element ))
    {
		CRenderableObject* lRenderableObject;
        for( uint32 i = 0, lCount = element.GetNumChildren(); i < lCount ; ++i)
        {
			CXMLTreeNode lSubElement = element(i);
			const std::string& lName = lSubElement.GetAttribute<std::string>("name", "" );
			lRenderableObject = SceneInstance->GetResource(lSubElement.GetAttribute<std::string>("room", ""))->GetLayer(lSubElement.GetAttribute<std::string>("layer", "solid"))->GetResource(lName);            
			m_vRenderableObject.push_back(lRenderableObject);
        }
    }
}

void CSetActiveElement::Update()
{
	std::vector<CRenderableObject*>::iterator it = m_vRenderableObject.begin(),
												   it_end = m_vRenderableObject.end();
	for(; it != it_end; ++it)
	{
		if((*it) == 0)
		{
			(*it) = SceneInstance->GetResource("core")->GetLayer("characters")->GetResource(m_Name);
		}
			(*it)->SetActive(m_bActive);
	}
	m_CurrentTime += deltaTimeMacro;
}

void CSetActiveElement::Execute( CGraphicsManager& GM )
{

}

void CSetActiveElement::Render()
{
}

void CSetActiveElement::Render2D(CGraphicsManager &GM)
{
}