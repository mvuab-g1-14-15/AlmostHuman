#ifndef ENABLEZ_ALPHA_TEST_SCENE_RENDERER_COMMAND_
#define ENABLEZ_ALPHA_TEST_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEnableAlphaTestSceneRendererCommand : public CSceneRendererCommand
{
public:
  CEnableAlphaTestSceneRendererCommand( CXMLTreeNode& atts );
  ~ CEnableAlphaTestSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif