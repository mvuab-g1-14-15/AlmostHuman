#include "SceneRenderComands\SetRenderTargetSceneRendererCommand.h"


CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand( CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
{
}

CSetRenderTargetSceneRendererCommand::~CSetRenderTargetSceneRendererCommand()
{
}

void CSetRenderTargetSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
    m_StageTextures[i].m_Texture->SetAsRenderTarget( m_StageTextures[i].m_StageId );
}

void CSetRenderTargetSceneRendererCommand::UnSetRenderTarget(int IdSetDepthStencilBuffer)
{
  for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
    m_StageTextures[i].m_Texture->UnsetAsRenderTarget( m_StageTextures[i].m_StageId, IdSetDepthStencilBuffer!=i);

  if ( false ) // DEBUG
		DebugTextures();
}
