#ifndef DISABLEZ_ALPHA_BLEND_SCENE_RENDERER_COMMAND_
#define DISABLEZ_ALPHA_BLEND_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CDisableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
  CDisableAlphaBlendSceneRendererCommand( CXMLTreeNode& atts );
  ~ CDisableAlphaBlendSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif