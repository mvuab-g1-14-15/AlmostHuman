#ifndef WEAPON_TARGET_RENDERER_COMMAND_
#define WEAPON_TARGET_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CLight;

class CWeaponTargetRendererCommand :  public CDrawQuadRendererCommand
{
public:
  CWeaponTargetRendererCommand( CXMLTreeNode& atts );
  void Execute( CGraphicsManager& GM );
private:
	uint32 mGizmoSize;
};
#endif // WEAPON_TARGET_RENDERER_COMMAND_