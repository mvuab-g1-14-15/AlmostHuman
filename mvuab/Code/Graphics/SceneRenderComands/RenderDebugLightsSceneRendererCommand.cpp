#include "SceneRenderComands\RenderDebugLightsSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
  , m_RenderLights( atts.GetBoolProperty( "render", false ) )
{
}

CRenderDebugLightsSceneRendererCommand::~CRenderDebugLightsSceneRendererCommand()
{

}

void CRenderDebugLightsSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  if ( m_RenderLights )
    CLightManager::GetSingletonPtr()->Render();
}