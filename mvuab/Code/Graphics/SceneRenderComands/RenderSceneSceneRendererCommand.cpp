#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "GraphicsManager.h"

#include "EngineManagers.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
    const std::string &l_LayerName = atts.GetAttribute<std::string>("layer", "");
    m_Layer = ROLMInstance->GetResource(l_LayerName);

    ASSERT(m_Layer, "Invalid layer %s", l_LayerName.c_str() );
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    m_Layer->Render();
}