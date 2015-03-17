#include "SceneRenderComands\RenderDebugPhysicsSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"

CRenderDebugPhysicsSceneRendererCommand::CRenderDebugPhysicsSceneRendererCommand(CXMLTreeNode &atts) : CSceneRendererCommand(atts)
{
}

void CRenderDebugPhysicsSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  CPhysicsManager::GetSingletonPtr()->DebugRender(&GM);
}