#ifndef RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include <vector>

class CGraphicsManager;
class CRenderableObjectsManager;

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand
{
	private:
		std::vector<CRenderableObjectsManager*> m_Layers;
    public:
        CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderDebugSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif