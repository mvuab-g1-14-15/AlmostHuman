#include "SceneRenderComands\DisableZTestSceneRendererCommand.h"
#include "GraphicsManager.h"

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CDisableZTestSceneRendererCommand::~CDisableZTestSceneRendererCommand()
{
    
}

void CDisableZTestSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	GM.DisableZTest();
}