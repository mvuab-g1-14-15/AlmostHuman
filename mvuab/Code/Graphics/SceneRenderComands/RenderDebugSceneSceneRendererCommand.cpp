#include "SceneRenderComands\RenderDebugSceneSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "EngineManagers.h"


CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
}

CRenderDebugSceneSceneRendererCommand::~CRenderDebugSceneSceneRendererCommand()
{
}

void CRenderDebugSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CRenderDebugSceneSceneRendererCommand::Execute", Profiler::Color::Orchid )
}