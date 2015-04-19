#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "EngineManagers.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
    const std::string &l_LayerName = atts.GetPszProperty("layer", "");
    m_Layer = ROLMInstance->GetResource(l_LayerName);
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    m_Layer->Render();
}