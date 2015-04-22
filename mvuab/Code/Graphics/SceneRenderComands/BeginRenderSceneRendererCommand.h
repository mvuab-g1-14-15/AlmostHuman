#ifndef BEGIN_RENDER_SCENE_RENDERER_COMMAND_
#define BEGIN_RENDER_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CBeginRenderSceneRendererCommand : public CSceneRendererCommand
{
	public:
        CBeginRenderSceneRendererCommand(CXMLTreeNode &atts);
		void Execute(CGraphicsManager &GM);
};
#endif