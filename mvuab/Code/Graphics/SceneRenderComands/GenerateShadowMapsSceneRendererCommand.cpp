#include "SceneRenderComands\GenerateShadowMapsSceneRendererCommand.h"
#include "GraphicsManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts)
    :CSceneRendererCommand(atts)
{
}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
    
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}