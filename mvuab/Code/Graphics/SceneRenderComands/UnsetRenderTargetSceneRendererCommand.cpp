#include "SceneRenderComands\UnsetRenderTargetSceneRendererCommand.h"


CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CUnsetRenderTargetSceneRendererCommand::~CUnsetRenderTargetSceneRendererCommand()
{
    
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}