#ifndef DISABLEZ_ALPHA_TEST_SCENE_RENDERER_COMMAND_
#define DISABLEZ_ALPHA_TEST_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CDisableAlphaTestSceneRendererCommand : public CSceneRendererCommand
{
public:
  CDisableAlphaTestSceneRendererCommand( CXMLTreeNode& atts );
  ~ CDisableAlphaTestSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif