#include "SceneRenderComands\RenderDebugCamerasCommand.h"
#include "GraphicsManager.h"
#include "Cameras/CameraManager.h"

CRenderDebugCamerasCommand::CRenderDebugCamerasCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
  , m_RenderCameras( atts.GetBoolProperty( "render", false ) )
{
}

CRenderDebugCamerasCommand::~CRenderDebugCamerasCommand()
{

}

void CRenderDebugCamerasCommand::Execute( CGraphicsManager& GM )
{
  if ( m_RenderCameras )
    CCameraManager::GetSingletonPtr()->RenderCameras();
}