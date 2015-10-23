#include "SceneRenderComands\TriggerRendererCommand.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Triggers/TriggerManager.h"

CTriggerRenderCommand::CTriggerRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CTriggerRenderCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CRenderDebugShadowMapsSceneRendererCommand::Execute", Profiler::Color::Orchid )
	TriggersMInstance->Render();
}