#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "GraphicsManager.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts)
    :CStagedTexturedRendererCommand(atts)
{
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
    
}


void CDeferredShadingSceneRendererCommand::SetLightsData(CGraphicsManager &GM)
{

}
void CDeferredShadingSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}