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
  BROFILER_CATEGORY( "CEnableZWriteSceneRendererCommand::Execute", Profiler::Color::Orchid )
  GM.EnableZWrite();
}