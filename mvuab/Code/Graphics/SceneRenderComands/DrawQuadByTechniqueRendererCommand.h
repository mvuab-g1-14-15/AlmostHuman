#ifndef DRAW_QUAD_BY_TECHNIQUE_RENDERER_COMMAND_
#define DRAW_QUAD_BY_TECHNIQUE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Math\Color.h"

class CGraphicsManager;
class CEffectTechnique;

class CDrawQuadByTechniqueRendererCommand : public CStagedTexturedRendererCommand
{
protected:
  Math::CColor m_Color;
  CEffectTechnique* m_Technique;
public:
  CDrawQuadByTechniqueRendererCommand( CXMLTreeNode& atts );
  ~ CDrawQuadByTechniqueRendererCommand();

  void Execute( CGraphicsManager& GM );
};
#endif