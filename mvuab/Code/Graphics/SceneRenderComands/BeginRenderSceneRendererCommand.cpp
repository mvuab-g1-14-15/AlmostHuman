#include "SceneRenderComands\BeginRenderSceneRendererCommand.h"
#include "GraphicsManager.h"

CBeginRenderSceneRendererCommand::CBeginRenderSceneRendererCommand(CXMLTreeNode &atts) : CSceneRendererCommand(atts)
{
}

void CBeginRenderSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	GM.BeginScene();
}