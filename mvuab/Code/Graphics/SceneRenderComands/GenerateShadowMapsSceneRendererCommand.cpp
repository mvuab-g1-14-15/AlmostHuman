#include "SceneRenderComands\GenerateShadowMapsSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Lights/LightManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(
  CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
{
}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  CLightManager::GetSingletonPtr()->GenerateShadowMap( GraphicsInstance );
}