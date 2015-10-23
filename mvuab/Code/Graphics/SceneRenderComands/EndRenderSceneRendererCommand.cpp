#include "SceneRenderComands\EndRenderSceneRendererCommand.h"
#include "GraphicsManager.h"

CEndRenderSceneRendererCommand::CEndRenderSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

CEndRenderSceneRendererCommand::~CEndRenderSceneRendererCommand()
{
    
}

void CEndRenderSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CEndRenderSceneRendererCommand::Execute", Profiler::Color::Orchid )
	GM.EndScene();
}