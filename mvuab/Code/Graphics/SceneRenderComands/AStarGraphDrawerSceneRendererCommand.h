#ifndef ASTAR_SCENE_RENDERER_COMMAND_
#define ASTAR_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CAStarGraphDrawerSceneRendererCommand : public CSceneRendererCommand
{
	public:
        CAStarGraphDrawerSceneRendererCommand(CXMLTreeNode &atts);
		void Execute(CGraphicsManager &GM);
};
#endif