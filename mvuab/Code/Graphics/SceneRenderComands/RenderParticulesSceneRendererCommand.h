#ifndef RENDER_PARTICULES_SCENE_RENDERER_COMMAND_
#define RENDER_PARTICULES_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderParticulesSceneRendererCommand : public CSceneRendererCommand
{
public:
  CRenderParticulesSceneRendererCommand( CXMLTreeNode& atts );
  ~ CRenderParticulesSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
private:

};
#endif //RENDER_PARTICULES_SCENE_RENDERER_COMMAND_