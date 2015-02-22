#include "SceneRenderComands\EnableAlphaBlendSceneRendererCommand.h"
#include "GraphicsManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(
  CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
  if ( atts.Exists() )
  {
    m_SrcBlend =  atts.GetPszProperty( "srcblend", "" );
    m_DstBlend =  atts.GetPszProperty( "dstblend", "" );
  }
}

CEnableAlphaBlendSceneRendererCommand::~CEnableAlphaBlendSceneRendererCommand()
{
}

void  CEnableAlphaBlendSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.EnableAlphaBlend();
  GM.EnableAlphaTest(); //TODO RAUL Split las sentenciqas en comandos separados
  GM.SetSrcBlend( m_SrcBlend );
  GM.SetDstBlend( m_DstBlend );
}