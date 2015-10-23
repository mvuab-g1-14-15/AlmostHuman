#include "SceneRenderComands\EnableAlphaBlendSceneRendererCommand.h"
#include "GraphicsManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
}

CEnableAlphaBlendSceneRendererCommand::~CEnableAlphaBlendSceneRendererCommand()
{
}

void  CEnableAlphaBlendSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CEnableAlphaBlendSceneRendererCommand::Execute", Profiler::Color::Orchid )
  GM.EnableAlphaBlend();
}