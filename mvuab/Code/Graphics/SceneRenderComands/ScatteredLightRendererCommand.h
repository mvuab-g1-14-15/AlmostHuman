#ifndef SCATERED_LIGHT_SCENE_RENDERER_COMMAND_
#define SCATERED_LIGHT_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CEffectTechnique;
class CTexture;

class CScatteredLightSceneRendererCommand : public CStagedTexturedRendererCommand
{
    private:
        CTexture m_RenderTarget1;
        CTexture m_RenderTarget2;

        float m_StartWeight;
        float m_EndWeight;

        float m_Time;
        float m_AccTime;

    public:
        CScatteredLightSceneRendererCommand( CXMLTreeNode& atts );
        ~ CScatteredLightSceneRendererCommand();

        void Execute(CGraphicsManager& GM);
        float MyLerp(float v1, float v2, float t);

        void TurnOff(float l_Time);
        void TurnOn(float l_Time, float l_Weigth = 5.65f);
};
#endif