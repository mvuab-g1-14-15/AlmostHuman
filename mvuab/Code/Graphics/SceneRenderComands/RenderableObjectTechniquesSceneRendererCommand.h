#ifndef REMDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_
#define REMDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include <string>

class CGraphicsManager;

class CRenderableObjectTechniquesSceneRendererCommand : public CSceneRendererCommand
{
	private:
		//CPoolRenderableObjectTechnique  *m_PoolRenderableObjectTechnique;
    public:
        CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts);
        ~CRenderableObjectTechniquesSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
};
#endif