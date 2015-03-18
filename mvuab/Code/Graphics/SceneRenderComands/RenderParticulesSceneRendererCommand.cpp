#include "SceneRenderComands\RenderParticulesSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "Billboard\Billboard.h"

CRenderParticulesSceneRendererCommand::CRenderParticulesSceneRendererCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
{
}

CRenderParticulesSceneRendererCommand::~CRenderParticulesSceneRendererCommand()
{

}

void CRenderParticulesSceneRendererCommand::Execute( CGraphicsManager& GM )
{
	CBillboard* bb = CCore::GetSingletonPtr()->GetBillboard();
	bb->Render();
}