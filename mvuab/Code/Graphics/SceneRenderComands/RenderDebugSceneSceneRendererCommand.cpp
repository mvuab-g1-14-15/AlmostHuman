#include "SceneRenderComands\RenderDebugSceneSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "EngineManagers.h"

CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(
        atts)
{
    const std::string &l_LayerName = atts.GetPszProperty("layer", "");
    m_Layer = ROLMInstance->GetResource(l_LayerName);
}

CRenderDebugSceneSceneRendererCommand::~CRenderDebugSceneSceneRendererCommand()
{
    CHECKED_DELETE(m_Layer);
}

void CRenderDebugSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    m_Layer->Render();
}