#ifndef LENS_OF_FLARE_RENDERER_COMMAND_
#define LENS_OF_FLARE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CLight;

class CLensOfFlareRendererCommand :  public CDrawQuadRendererCommand
{
public:
  CLensOfFlareRendererCommand( CXMLTreeNode& atts );
  void Execute( CGraphicsManager& GM );
private:
  CLight* GetSun();
};
#endif