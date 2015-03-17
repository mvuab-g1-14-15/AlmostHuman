#ifndef PHYSICS_RENDERER_COMMAND_
#define PHYSICS_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderDebugPhysicsSceneRendererCommand : public CSceneRendererCommand
{
	public:
        CRenderDebugPhysicsSceneRendererCommand(CXMLTreeNode &atts);
		void Execute(CGraphicsManager &GM);
};
#endif