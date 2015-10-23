#include "SceneRenderComands\DisableZWriteSceneRendererCommand.h"
#include "GraphicsManager.h"

CDisableZWriteSceneRendererCommand::CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CDisableZWriteSceneRendererCommand::~CDisableZWriteSceneRendererCommand()
{
    
}
void CDisableZWriteSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CDisableZWriteSceneRendererCommand::Execute", Profiler::Color::Orchid )
	GM.DisableZWrite();
}