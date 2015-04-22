#ifndef SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_
#define SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
  CSetRenderTargetSceneRendererCommand( CXMLTreeNode& atts );
  ~CSetRenderTargetSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
  void UnSetRenderTarget();
};
#endif