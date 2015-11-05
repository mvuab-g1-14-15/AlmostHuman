#ifndef ZBLUR_SCENE_RENDERER_COMMAND_
#define ZBLUR_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CEffectTechnique;

class CZBlurSceneRendererCommand : public CStagedTexturedRendererCommand
{
    private:
        float m_FocalStart1;
        float m_FocalStart2;

        float m_FocalEnd1;
        float m_FocalEnd2;

        float m_BlurConstant1;
        float m_BlurConstant2;

        float m_BlurEnd1;
        float m_BlurEnd2;

        float m_Time;
        float m_AccTime;

    public:
        CZBlurSceneRendererCommand( CXMLTreeNode& atts );
        ~ CZBlurSceneRendererCommand();

        void Execute(CGraphicsManager& GM);
        float LerpAndClamp(float v1, float v2, float t);
        void AdjustLens(float l_FocalStart, float l_FocalEnd, float l_BlurConstant, float l_BlurEnd, float l_Time);
};
#endif