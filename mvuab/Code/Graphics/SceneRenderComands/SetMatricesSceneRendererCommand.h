#ifndef SET_MATRICES_SCENE_RENDERER_COMMAND_
#define SET_MATRICES_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CSetMatricesSceneRendererCommand(CXMLTreeNode &atts);
        ~CSetMatricesSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif