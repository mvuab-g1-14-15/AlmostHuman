#ifndef ENABLE_SET_BLEND_SCENE_RENDERER_COMMAND_
#define ENABLE_SET_BLEND_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSetBlendSceneRendererCommand : public CSceneRendererCommand
{
private:
  std::string m_SrcBlend;
  std::string m_DstBlend;
public:
  CSetBlendSceneRendererCommand( CXMLTreeNode& atts );
  ~ CSetBlendSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif