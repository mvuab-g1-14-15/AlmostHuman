#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "GraphicsManager.h"
#include "Core.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts): CSceneRendererCommand(atts)
{
	const std::string &l_LayerName = atts.GetPszProperty("name", "");
	m_Layer = CCore::GetSingletonPtr()->GetRenderableObjectsLayersManager()->GetResource(l_LayerName);
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
	CHECKED_DELETE(m_Layer);
}

void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	m_Layer->Render();
}