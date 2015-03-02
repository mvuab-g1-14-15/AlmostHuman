#include "SceneRenderComands\SetBlendSceneRendererCommand.h"
#include "GraphicsManager.h"

CSetBlendSceneRendererCommand::CSetBlendSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
  if ( atts.Exists() )
  {
    m_SrcBlend =  atts.GetPszProperty( "srcblend", "" );
    m_DstBlend =  atts.GetPszProperty( "dstblend", "" );
  }
}

CSetBlendSceneRendererCommand::~CSetBlendSceneRendererCommand()
{
}

void  CSetBlendSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.SetSrcBlend( m_SrcBlend );
  GM.SetDstBlend( m_DstBlend );
}