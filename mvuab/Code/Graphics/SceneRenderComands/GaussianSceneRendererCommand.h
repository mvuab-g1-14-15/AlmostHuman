#ifndef GAUSSIAN_SCENE_RENDERER_COMMAND_
#define GAUSSIAN_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CGaussianSceneRendererCommand : public CSceneRendererCommand
{
private:
  std::string m_NameTechnique;
  float m_Weight[5];
  float m_Offset[5];
public:
  CGaussianSceneRendererCommand( CXMLTreeNode& atts );
  ~ CGaussianSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif