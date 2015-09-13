#include "SceneRenderComands\CaptureFrameBufferSceneRendererCommand.h"
#include "GraphicsManager.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(
  CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
{
}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
    m_StageTextures[i].m_Texture->CaptureFrameBuffer( m_StageTextures[i].m_StageId );

  if ( false )
    m_StageTextures[0].m_Texture->Save( "capture_fb" + GetName() );
}