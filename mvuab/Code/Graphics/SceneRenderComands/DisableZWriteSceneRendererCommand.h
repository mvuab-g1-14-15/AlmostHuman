#ifndef DISABLEZ_WRITE_SCENE_RENDERER_COMMAND_
#define DISABLEZ_WRITE_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CDisableZWriteSceneRendererCommand : public CSceneRendererCommand
{
    public:
        CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts);
        ~ CDisableZWriteSceneRendererCommand();

		void Execute(CGraphicsManager &GM);
};
#endif