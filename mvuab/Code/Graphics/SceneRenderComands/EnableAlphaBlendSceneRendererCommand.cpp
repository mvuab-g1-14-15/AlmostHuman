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
  GM.EnableAlphaBlend();
}