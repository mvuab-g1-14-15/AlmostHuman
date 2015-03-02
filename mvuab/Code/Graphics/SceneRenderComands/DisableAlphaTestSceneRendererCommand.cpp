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
  GM.DisableAlphaTest();
}