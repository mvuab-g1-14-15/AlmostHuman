#ifndef RENDER_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_SCENE_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"

class CGraphicsManager;
class CScene;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
	private:
	 CScene* mScene;
	 std::string mLayer;

    public:
        CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif