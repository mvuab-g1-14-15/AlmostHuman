#ifndef RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_
#define RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderDebugLightsSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderDebugLightsSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif