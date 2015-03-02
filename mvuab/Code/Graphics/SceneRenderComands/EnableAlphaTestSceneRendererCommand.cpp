#include "SceneRenderComands\EnableAlphaTestSceneRendererCommand.h"
#include "GraphicsManager.h"

CEnableAlphaTestSceneRendererCommand::CEnableAlphaTestSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
}

CEnableAlphaTestSceneRendererCommand::~CEnableAlphaTestSceneRendererCommand()
{
}

void  CEnableAlphaTestSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.EnableAlphaTest();
}