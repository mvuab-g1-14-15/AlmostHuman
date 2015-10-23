#include "SceneRenderComands\EnableZTestSceneRendererCommand.h"
#include "GraphicsManager.h"

CEnableZTestSceneRendererCommand::CEnableZTestSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CEnableZTestSceneRendererCommand::~CEnableZTestSceneRendererCommand()
{
    
}

void CEnableZTestSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CEnableZTestSceneRendererCommand::Execute", Profiler::Color::Orchid )
	GM.EnableZTest();
}