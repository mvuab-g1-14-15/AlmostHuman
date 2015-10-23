#include "SceneRenderComands\GenerateShadowMapsSceneRendererCommand.h"

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
  BROFILER_CATEGORY( "CGenerateShadowMapsSceneRendererCommand::Execute", Profiler::Color::Orchid )
    LightMInstance->GenerateShadowMap( GraphicsInstance );
}