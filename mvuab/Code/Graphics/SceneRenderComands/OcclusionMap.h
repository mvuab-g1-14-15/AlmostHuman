#ifndef OCCLUSION_MAP_SCENE_RENDERER_COMMAND_
#define OCCLUSION_MAP_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CEffectTechnique;
class CTexture;

class COcclusionMapSceneRendererCommand : public CStagedTexturedRendererCommand
{
    public:
        COcclusionMapSceneRendererCommand( CXMLTreeNode& atts );
        ~COcclusionMapSceneRendererCommand();

        void Execute( CGraphicsManager& GM );

    private:
     
};
#endif