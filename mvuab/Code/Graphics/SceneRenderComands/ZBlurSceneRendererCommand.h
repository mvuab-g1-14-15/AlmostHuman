#ifndef ZBLUR_SCENE_RENDERER_COMMAND_
#define ZBLUR_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CEffectTechnique;

class CZBlurSceneRendererCommand : public CStagedTexturedRendererCommand
{
    public:
        CZBlurSceneRendererCommand( CXMLTreeNode& atts );
        ~ CZBlurSceneRendererCommand();

        void Execute( CGraphicsManager& GM );

    private:
        float m_FocalStart;
        float m_FocalEnd;

        float m_BlurConstant;
        float m_BlurEnd;

};
#endif