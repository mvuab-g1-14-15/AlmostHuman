#include "SceneRenderComands\PresentSceneRendererCommand.h"
#include "GraphicsManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CPresentSceneRendererCommand::~CPresentSceneRendererCommand()
{
    
}

void CPresentSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CPresentSceneRendererCommand::Execute", Profiler::Color::Orchid )
	GM.Present();
}