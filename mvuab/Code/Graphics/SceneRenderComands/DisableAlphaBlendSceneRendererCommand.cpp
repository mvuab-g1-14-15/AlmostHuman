#include "SceneRenderComands\DisableAlphaBlendSceneRendererCommand.h"
#include "GraphicsManager.h"

CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
}

CDisableAlphaBlendSceneRendererCommand::~CDisableAlphaBlendSceneRendererCommand()
{
}
void CDisableAlphaBlendSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.DisableAlphaBlend();
}