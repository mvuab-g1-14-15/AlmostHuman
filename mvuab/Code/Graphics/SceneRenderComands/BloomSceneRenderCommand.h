#pragma once

#ifndef _BLOOM_SCENE_RENDER_COMMAND_H
#define _BLOOM_SCENE_RENDER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CGraphicsManager;
class CXMLTreeNode;
class CBloomProcessSceneRenderCommand;
//-------------------------//

class CBloomSceneRenderCommand : public CSceneRendererCommand
{
public:
  //--- Init and End protocols------------------------------------------
  CBloomSceneRenderCommand( CXMLTreeNode& _Node );
  virtual     ~CBloomSceneRenderCommand( void );

  //----Main Methods ---------------------------------------------------
  virtual void  Execute( CGraphicsManager& GM );

  //----Members --------------------------------------------------------
private:
  CBloomProcessSceneRenderCommand*    m_Bloom;
};

#endif