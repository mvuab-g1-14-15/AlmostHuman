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
	GM.DisableZWrite();
}