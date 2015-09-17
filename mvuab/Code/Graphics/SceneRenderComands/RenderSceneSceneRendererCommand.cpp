#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "GraphicsManager.h"

#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "EngineManagers.h"

#include <algorithm>


CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
    const std::string &l_LayerName = atts.GetAttribute<std::string>("layer", "");
    
	CScene* l_Scene = SceneInstance;

	std::map<std::string, CRoom*> l_SceneMap = l_Scene->GetResourcesMap();
	std::map<std::string, CRoom*>::iterator it = l_SceneMap.begin(), it_end = l_SceneMap.end();

	for (;it!=it_end; ++it)
	{
		CRoom* lRoom = it->second;
		
		if (lRoom->GetActive())
		{
			CRenderableObjectsManager* lROM = lRoom->GetLayers()->GetResource(l_LayerName);
			if (lROM)
				AddLayer(lROM);
		}
	}
}

void CRenderSceneSceneRendererCommand::AddLayer( CRenderableObjectsManager* aROM )
{
	m_Layers.push_back(aROM);
}

bool CRenderSceneSceneRendererCommand::RemoveLayer( CRenderableObjectsManager* aROM )
{
    std::vector<CRenderableObjectsManager*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), aROM);
    if(it != m_Layers.end()){ m_Layers.erase(it); return true; }

    return false;
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	std::vector<CRenderableObjectsManager*>::iterator it = m_Layers.begin(), it_end = m_Layers.end();

	for (;it!=it_end; ++it)
	{
		(*it)->Render();
	}
}