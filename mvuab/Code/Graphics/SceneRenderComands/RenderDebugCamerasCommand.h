#ifndef RENDER_DEBUG_CAMERAS_COMMAND_
#define RENDER_DEBUG_CAMERAS_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderDebugCamerasCommand : public CSceneRendererCommand
{
public:
  CRenderDebugCamerasCommand( CXMLTreeNode& atts );
  ~ CRenderDebugCamerasCommand();

  void Execute( CGraphicsManager& GM );
private:

  bool m_RenderCameras;
};
#endif