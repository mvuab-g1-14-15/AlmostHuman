#ifndef RENDER_DEBUG_COMMAND_
#define RENDER_DEBUG_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderDebugCommand : public CSceneRendererCommand
{
public:
  CRenderDebugCommand( CXMLTreeNode& atts );
  ~ CRenderDebugCommand();

  void Execute( CGraphicsManager& GM );
private:

  bool m_RenderCameras;
  bool m_RenderLights;
  bool m_RenderPhysics;
  bool m_RenderGraph;
};
#endif