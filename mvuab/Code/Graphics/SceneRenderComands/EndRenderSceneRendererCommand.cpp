#include "SceneRenderComands\EndRenderSceneRendererCommand.h"


CEndRenderSceneRendererCommand::CEndRenderSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

CEndRenderSceneRendererCommand::~CEndRenderSceneRendererCommand()
{
    
}

void CEndRenderSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}