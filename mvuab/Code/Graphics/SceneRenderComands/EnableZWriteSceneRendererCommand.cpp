#include "SceneRenderComands\EnableZWriteSceneRendererCommand.h"
#include "GraphicsManager.h"

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand( CXMLTreeNode& atts ):
  CSceneRendererCommand( atts )
{
}

CEnableZWriteSceneRendererCommand::~CEnableZWriteSceneRendererCommand()
{
}

void  CEnableZWriteSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.EnableZWrite();
}