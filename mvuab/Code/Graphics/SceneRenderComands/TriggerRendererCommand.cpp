#include "SceneRenderComands\TriggerRendererCommand.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Triggers/TriggerManager.h"

CTriggerRenderCommand::CTriggerRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CTriggerRenderCommand::Execute( CGraphicsManager& GM )
{
	TriggersMInstance->Render();
}