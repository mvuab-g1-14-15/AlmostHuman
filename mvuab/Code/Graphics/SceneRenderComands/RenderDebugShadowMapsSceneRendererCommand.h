#ifndef RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_
#define RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderDebugShadowMapsSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif