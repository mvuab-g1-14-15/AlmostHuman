#ifndef PROCESS_RENDER_SCENE_RENDERER_COMMAND_
#define PROCESS_RENDER_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CProcessRenderRendererCommand : public CSceneRendererCommand
{
    public:
        CProcessRenderRendererCommand(CXMLTreeNode &atts);
        void Execute(CGraphicsManager &GM);
};
#endif // PROCESS_RENDER_SCENE_RENDERER_COMMAND_