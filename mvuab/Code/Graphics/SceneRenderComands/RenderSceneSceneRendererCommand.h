#ifndef RENDER_SCENE_SCENE_RENDERER_COMMAND_
#define RENDER_SCENE_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include <vector>

class CGraphicsManager;
class CRenderableObjectsManager;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
	private:
	 std::vector<CRenderableObjectsManager*> m_Layers;

    public:
        CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);
        ~ CRenderSceneSceneRendererCommand();

        void Execute(CGraphicsManager &GM);
		void AddLayer( CRenderableObjectsManager* aROM );
        bool RemoveLayer( CRenderableObjectsManager* aROM );
};
#endif