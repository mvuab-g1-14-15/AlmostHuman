#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
    public:
        CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts);
        ~ CSetRenderTargetSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
        void UnSetRenderTarget();
};
#endif