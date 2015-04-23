#ifndef DEVELOPER_INFO_SCENE_RENDERER_COMMAND_
#define DEVELOPER_INFO_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CDeveloperInfoSceneRenderCommand: public CSceneRendererCommand
{
public:
  CDeveloperInfoSceneRenderCommand( CXMLTreeNode& atts );
  void Execute( CGraphicsManager& GM );
private:
  Math::CColor m_Quad2dColor;
  Math::CColor m_Quad2dEdgeColor;
};
#endif