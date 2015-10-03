#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils\Defines.h"

#include "RenderableObject\Scene.h"


CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
	, mScene( SceneInstance )
	, mLayer( atts.GetAttribute<std::string>("layer", "") )
{

}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	mScene->RenderLayer( mLayer );
}