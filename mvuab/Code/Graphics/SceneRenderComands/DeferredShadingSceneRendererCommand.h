#ifndef DEFERRED_SHADING_SCENE_RENDERER_COMMAND_
#define DEFERRED_SHADING_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;


class CDeferredShadingSceneRendererCommand : public CStagedTexturedRendererCommand
{
	private:
        //CRenderableObjectTechnique  *m_RenderableObjectTechnique;

        void SetLightsData(CGraphicsManager &GM);
    public:
        CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts);
        ~ CDeferredShadingSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif