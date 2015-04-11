#include "SceneRenderComands\GenerateShadowMapsSceneRendererCommand.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Lights/LightManager.h"
#include "EngineManagers.h"

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
  LightMInstance->GenerateShadowMap( GraphicsInstance );
}