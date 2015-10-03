#ifndef RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
class CGraphicsManager;

class CRenderDebugSceneSceneRendererCommand : public CRenderSceneSceneRendererCommand
{
    public:
        CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderDebugSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif