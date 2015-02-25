#ifndef GAUSSIAN_SCENE_RENDERER_COMMAND_
#define GAUSSIAN_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CGaussianSceneRendererCommand : public CStagedTexturedRendererCommand
{
private:
  int    m_nIteration;
  uint32 m_Width;
  uint32 m_Height;
public:
  CGaussianSceneRendererCommand( CXMLTreeNode& atts );
  ~ CGaussianSceneRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif