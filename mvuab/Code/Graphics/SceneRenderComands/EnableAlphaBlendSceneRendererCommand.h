#ifndef ENABLEZ_ALPHA_BLEND_SCENE_RENDERER_COMMAND_
#define ENABLEZ_ALPHA_BLEND_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{

public:
  CEnableAlphaBlendSceneRendererCommand( CXMLTreeNode& atts );
  ~ CEnableAlphaBlendSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif