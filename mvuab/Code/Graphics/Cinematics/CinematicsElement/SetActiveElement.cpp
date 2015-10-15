#include "Cinematics\CinematicsElement\SetActiveElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

CSetActiveElement::CSetActiveElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_bActive( atts.GetAttribute<bool>("active", false ) )
{
	CXMLTreeNode newFile, element;
	if ( newFile.LoadAndFindNode( atts.GetAttribute<std::string>("resource_id_file", "").c_str(), "elements" , element ))
    {
		CRenderableObject* l_RenderableObject;
        for( uint32 i = 0, lCount = element.GetNumChildren(); i < lCount ; ++i)
        {
			l_RenderableObject = SceneInstance->GetResource(element(i).GetAttribute<std::string>("room", ""))->GetLayer("solid")->GetResource(element(i).GetAttribute<std::string>("name", ""));            
			m_vRenderableObject.push_back(l_RenderableObject);
        }
    }
}

void CSetActiveElement::Update()
{
	std::vector<CRenderableObject*>::iterator it = m_vRenderableObject.begin(),
												   it_end = m_vRenderableObject.end();
	for(; it != it_end; ++it)
	{
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