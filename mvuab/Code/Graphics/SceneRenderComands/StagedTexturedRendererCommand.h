#ifndef STAGED_TEXTURED_RENDERER_COMMAND_
#define STAGED_TEXTURED_RENDERER_COMMAND_
#pragma once


#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"

#include <vector>

class CGraphicsManager;

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:
  class CKGStageTexture
  {
  public:
    int     m_StageId;
    CTexture*  m_Texture;
    CKGStageTexture( int StageId, CTexture* Texture )
    {
      m_StageId = StageId;
      m_Texture = Texture;
    }
    void Activate();
  };

  std::vector<CKGStageTexture>    m_StageTextures;
public:
  CStagedTexturedRendererCommand( CXMLTreeNode& atts );
  virtual ~CStagedTexturedRendererCommand();
  void ActivateTextures();
  void AddStageTexture( int StageId, CTexture* Texture );

  virtual void Execute( CGraphicsManager& GM ) = 0;
};
#endif