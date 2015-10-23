#include "SceneRenderComands\DisableAlphaTestSceneRendererCommand.h"
#include "GraphicsManager.h"

CDisableAlphaTestSceneRendererCommand::CDisableAlphaTestSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
}

CDisableAlphaTestSceneRendererCommand::~CDisableAlphaTestSceneRendererCommand()
{
}
void CDisableAlphaTestSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CDisableAlphaTestSceneRendererCommand::Execute", Profiler::Color::Orchid )
  GM.DisableAlphaTest();
}