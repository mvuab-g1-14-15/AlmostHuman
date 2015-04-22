#ifndef ENABLEZ_TEST_SCENE_RENDERER_COMMAND_
#define ENABLEZ_TEST_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEnableZTestSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CEnableZTestSceneRendererCommand(CXMLTreeNode &atts);
        ~ CEnableZTestSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif