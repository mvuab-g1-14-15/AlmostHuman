#include "SceneRenderComands\RenderDebugCommand.h"
#include "GraphicsManager.h"
#include "Cameras/CameraManager.h"
#include "PathFinding\AStar.h"
#include "Lights\LightManager.h"
#include "Engine.h"
#include "PhysicsManager.h"

CRenderDebugCommand::CRenderDebugCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
  , m_RenderCameras( atts.GetBoolProperty( "render_cameras", false ) )
  , m_RenderLights( atts.GetBoolProperty( "render_lights", false ) )
  , m_RenderPhysics( atts.GetBoolProperty( "render_physics", false ) )
  , m_RenderGraph( atts.GetBoolProperty( "render_graph", false ) )
{
}

CRenderDebugCommand::~CRenderDebugCommand()
{
}

void CRenderDebugCommand::Execute( CGraphicsManager& GM )
{
  if ( m_RenderCameras )
    CCameraManager::GetSingletonPtr()->RenderCameras();

  if ( m_RenderLights )
    CLightManager::GetSingletonPtr()->Render();

  if ( m_RenderGraph )
  {
    CAStar* lAStar = CEngine::GetSingletonPtr()->GetProcess()->GetAStar();

    if ( lAStar )
      lAStar->Render();
  }

  if ( m_RenderPhysics )
    CPhysicsManager::GetSingletonPtr()->DebugRender( &GM );
}