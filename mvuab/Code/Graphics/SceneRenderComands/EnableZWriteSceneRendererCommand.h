#ifndef ENABLEZ_WRITE_SCENE_RENDERER_COMMAND_
#define ENABLEZ_WRITE_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEnableZWriteSceneRendererCommand : public CSceneRendererCommand
{
	
    public:
        CEnableZWriteSceneRendererCommand(CXMLTreeNode &atts);
        ~ CEnableZWriteSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif