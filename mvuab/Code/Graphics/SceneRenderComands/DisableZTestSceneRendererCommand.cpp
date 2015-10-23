#include "SceneRenderComands\DisableZTestSceneRendererCommand.h"
#include "GraphicsManager.h"

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CDisableZTestSceneRendererCommand::~CDisableZTestSceneRendererCommand()
{
    
}

void CDisableZTestSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CDisableZTestSceneRendererCommand::Execute", Profiler::Color::Orchid )
	GM.DisableZTest();
}