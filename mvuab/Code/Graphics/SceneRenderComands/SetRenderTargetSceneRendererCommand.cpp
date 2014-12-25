#include "SceneRenderComands\SetRenderTargetSceneRendererCommand.h"


CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts)
    :CStagedTexturedRendererCommand(atts)
{

}

CSetRenderTargetSceneRendererCommand::~CSetRenderTargetSceneRendererCommand()
{
    
}

void CSetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}

void CSetRenderTargetSceneRendererCommand::UnSetRenderTarget()
{

}