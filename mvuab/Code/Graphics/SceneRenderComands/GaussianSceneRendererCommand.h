#ifndef GAUSSIAN_SCENE_RENDERER_COMMAND_
#define GAUSSIAN_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CGaussianSceneRendererCommand : public CStagedTexturedRendererCommand
{
    private:
        std::string m_NameTechnique;
        unsigned int m_nIteration;
        bool m_TextureAsFB;
        
        unsigned int m_Width;
        unsigned int m_Height;

    public:
        CGaussianSceneRendererCommand( CXMLTreeNode& atts );
        ~ CGaussianSceneRendererCommand();
        
        void Execute( CGraphicsManager& GM );
};
#endif