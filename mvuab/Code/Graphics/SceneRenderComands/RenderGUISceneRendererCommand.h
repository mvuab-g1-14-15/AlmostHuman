#ifndef RENDER_GUI_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_GUI_SCENE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderGUISceneRendererCommand: public CSceneRendererCommand
{
	private:
//		CGUI	*m_GUI;
    public:
        CRenderGUISceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderGUISceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif