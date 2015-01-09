#ifndef RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObject\RenderableObjectsManager.h"

class CGraphicsManager;

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand
{
	private:
		CRenderableObjectsManager	*m_Layer;
    public:
        CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderDebugSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif