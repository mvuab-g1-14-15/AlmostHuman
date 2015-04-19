#include "SceneRenderComands\SetMatricesSceneRendererCommand.h"


#include "GraphicsManager.h"
#include "EngineManagers.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CSetMatricesSceneRendererCommand::~CSetMatricesSceneRendererCommand()
{

}

void CSetMatricesSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    GraphicsInstance->SetupMatrices();
}