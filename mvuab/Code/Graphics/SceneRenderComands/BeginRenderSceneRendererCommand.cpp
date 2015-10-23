#include "SceneRenderComands\BeginRenderSceneRendererCommand.h"
#include "GraphicsManager.h"

CBeginRenderSceneRendererCommand::CBeginRenderSceneRendererCommand(CXMLTreeNode &atts) : CSceneRendererCommand(atts)
{

}


void CBeginRenderSceneRendererCommand::Execute(CGraphicsManager &GM)
{
  BROFILER_CATEGORY( "CBeginRender::Execute", Profiler::Color::Orchid )
	GM.BeginScene();
}