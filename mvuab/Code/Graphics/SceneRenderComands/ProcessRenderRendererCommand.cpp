#include "SceneRenderComands\ProcessRenderRendererCommand.h"
#include "GraphicsManager.h"
#include "Engine.h"

CProcessRenderRendererCommand::CProcessRenderRendererCommand(CXMLTreeNode &atts) : CSceneRendererCommand(atts)
{

}

void CProcessRenderRendererCommand::Execute(CGraphicsManager &GM)
{
    CEngine::GetSingletonPtr()->GetProcess()->Render();
}