#include "SceneRenderComands\SetMatricesSceneRendererCommand.h"

#include "Core.h"
#include "GraphicsManager.h"

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