#ifndef END_RENDER_SCENE_RENDERER_COMMAND_
#define END_RENDER_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEndRenderSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CEndRenderSceneRendererCommand(CXMLTreeNode &atts);
        ~ CEndRenderSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif