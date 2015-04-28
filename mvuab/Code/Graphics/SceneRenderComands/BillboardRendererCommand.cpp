#include "SceneRenderComands\BillboardRendererCommand.h"
#include "Billboard\BillboardManager.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CBillboardRenderCommand::CBillboardRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CBillboardRenderCommand::Execute( CGraphicsManager& GM )
{
	BillboardMan->Render();
}