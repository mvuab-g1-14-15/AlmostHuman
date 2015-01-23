#ifndef RENDER_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_SCENE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CRenderableObjectsManager;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
	private:
		CRenderableObjectsManager	*m_Layer;
    public:
        CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif