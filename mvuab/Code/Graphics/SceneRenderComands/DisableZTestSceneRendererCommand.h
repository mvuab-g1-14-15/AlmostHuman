#ifndef DISABLEZ_TEST_SCENE_RENDERER_COMMAND_
#define DISABLEZ_TEST_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CDisableZTestSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CDisableZTestSceneRendererCommand(CXMLTreeNode &atts);
        ~ CDisableZTestSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif