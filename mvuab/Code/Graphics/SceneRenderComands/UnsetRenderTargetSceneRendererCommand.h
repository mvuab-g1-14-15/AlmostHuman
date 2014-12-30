#ifndef UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_
#define UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "SceneRenderComands\SetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CUnsetRenderTargetSceneRendererCommand: public CSceneRendererCommand 
{
    private:
        CSetRenderTargetSceneRendererCommand  *m_SetRenderTargetRendererCommand;
    public:
        CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts); 
        ~CUnsetRenderTargetSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif