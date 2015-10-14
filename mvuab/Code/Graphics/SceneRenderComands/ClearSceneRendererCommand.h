#ifndef CLEAR_SCENE_RENDERER_COMMAND_
#define CLEAR_SCENE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
protected:
  bool  m_Color;
  bool  m_Depht;
  bool  m_Stencil;
  Math::CColor m_ClearColor;

public:
  CClearSceneRendererCommand( CXMLTreeNode& atts );
  virtual void Execute( CGraphicsManager& GM );
};
#endif