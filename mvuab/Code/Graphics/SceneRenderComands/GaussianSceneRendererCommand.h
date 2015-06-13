#ifndef GAUSSIAN_SCENE_RENDERER_COMMAND_
#define GAUSSIAN_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CEffectTechnique;

class CGaussianSceneRendererCommand : public CStagedTexturedRendererCommand
{
    public:
        CGaussianSceneRendererCommand( CXMLTreeNode& atts );
        ~ CGaussianSceneRendererCommand();

        void Execute( CGraphicsManager& GM );
    private:
        uint32 mIterations;
        uint32 m_Width;
        uint32 m_Height;

};
#endif