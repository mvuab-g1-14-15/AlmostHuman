#include "SceneRenderComands\LensOfFlareRendererCommand.h"
#include "GraphicsManager.h"

#include "EngineManagers.h"
#include "LensFlare/LensFlareManager.h"

CLensOfFlareRendererCommand::CLensOfFlareRendererCommand( CXMLTreeNode& atts ) :
    CDrawQuadRendererCommand( atts )
{
}

void CLensOfFlareRendererCommand::Execute( CGraphicsManager& GM )
{
    FlareMan->Render();
}