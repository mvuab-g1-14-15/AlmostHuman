#ifndef RENDER_GUI_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_GUI_SCENE_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CGUIManager;

class CRenderGUISceneRendererCommand: public CSceneRendererCommand
{
	private:
		CGUIManager	*m_GUI;
        bool         m_RenderGui;
    public:
        CRenderGUISceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderGUISceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif