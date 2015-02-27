#ifndef LENS_OF_FLARE_RENDERER_COMMAND_
#define LENS_OF_FLARE_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CLensOfFlareRendererCommand :  public CStagedTexturedRendererCommand
{
public:
  CLensOfFlareRendererCommand( CXMLTreeNode& atts );
  void Execute( CGraphicsManager& GM );
};
#endif