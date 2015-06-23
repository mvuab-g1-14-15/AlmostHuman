#include "SceneRenderComands\UnsetRenderTargetSceneRendererCommand.h"


CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand
        *SetRenderTargetRendererCommand, CXMLTreeNode &atts): CSceneRendererCommand(atts),
    m_SetRenderTargetRendererCommand(SetRenderTargetRendererCommand)
{

    m_UseDepthStencilBuffer = atts.GetAttribute<int32>("use_depth_stencil_buffer", -1);
}

CUnsetRenderTargetSceneRendererCommand::~CUnsetRenderTargetSceneRendererCommand()
{

}

void CUnsetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    m_SetRenderTargetRendererCommand->UnSetRenderTarget(m_UseDepthStencilBuffer);
}