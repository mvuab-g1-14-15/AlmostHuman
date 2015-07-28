#include "SceneRenderComands\RenderDebugSceneSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "EngineManagers.h"


CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
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
			if (lROM )
				m_Layers.push_back(lROM);
		}
		
	
	}

	ASSERT(m_Layers.size()!=0, "Invalid layer %s", l_LayerName.c_str() );
}

CRenderDebugSceneSceneRendererCommand::~CRenderDebugSceneSceneRendererCommand()
{
}

void CRenderDebugSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	std::vector<CRenderableObjectsManager*>::iterator it = m_Layers.begin(), it_end = m_Layers.end();

	for (;it!=it_end; ++it)
	{
		(*it)->Render();
	}
}