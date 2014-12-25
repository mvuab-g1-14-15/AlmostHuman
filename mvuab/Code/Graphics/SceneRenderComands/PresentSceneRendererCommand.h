#ifndef PRESENT_SCENE_RENDERER_COMMAND_
#define PRESENT_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CPresentSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CPresentSceneRendererCommand(CXMLTreeNode &atts);
        ~ CPresentSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif