#ifndef DRAW_QUAD_RENDERER_COMMAND_
#define DRAW_QUAD_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Math\Color.h"

class CGraphicsManager;

class CDrawQuadRendererCommand : public CStagedTexturedRendererCommand
{
protected:
  Math::CColor m_Color;
public:
  CDrawQuadRendererCommand( CXMLTreeNode& atts );
  ~ CDrawQuadRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif