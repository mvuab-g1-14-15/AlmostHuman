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
        CTexture m_RenderTarget3;        

    public:
        CScatteredLightSceneRendererCommand( CXMLTreeNode& atts );
        ~ CScatteredLightSceneRendererCommand();

        void Execute( CGraphicsManager& GM );
};
#endif