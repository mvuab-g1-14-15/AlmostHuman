#include "SceneRenderComands\AStarGraphDrawerSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Engine.h"
#include "PathFinding\AStar.h"

CAStarGraphDrawerSceneRendererCommand::CAStarGraphDrawerSceneRendererCommand(CXMLTreeNode &atts) : CSceneRendererCommand(atts)
{
}

void CAStarGraphDrawerSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	CAStar* lAStar = CEngine::GetSingletonPtr()->GetProcess()->GetAStar();

	if( lAStar )
	{
		lAStar->Render();
	}
}