#ifndef CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_
#define CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedRendererCommand
{
    public:
        CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts);
        ~ CCaptureFrameBufferSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif