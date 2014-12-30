#ifndef GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_
#define GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
        ~ CGenerateShadowMapsSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif