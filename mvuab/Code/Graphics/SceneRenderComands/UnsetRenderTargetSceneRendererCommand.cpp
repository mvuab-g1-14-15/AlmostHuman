#include "SceneRenderComands\UnsetRenderTargetSceneRendererCommand.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts): CSceneRendererCommand(atts), m_SetRenderTargetRendererCommand(SetRenderTargetRendererCommand)
{
}

CUnsetRenderTargetSceneRendererCommand::~CUnsetRenderTargetSceneRendererCommand()
{
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	m_SetRenderTargetRendererCommand->UnSetRenderTarget();
}