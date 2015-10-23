#include "SceneRenderComands\RenderDebugShadowMapsSceneRendererCommand.h"


CRenderDebugShadowMapsSceneRendererCommand::CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
    :CSceneRendererCommand(atts)
{
}

CRenderDebugShadowMapsSceneRendererCommand::~CRenderDebugShadowMapsSceneRendererCommand()
{
    
}

void CRenderDebugShadowMapsSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CRenderDebugShadowMapsSceneRendererCommand::Execute", Profiler::Color::Orchid )
}