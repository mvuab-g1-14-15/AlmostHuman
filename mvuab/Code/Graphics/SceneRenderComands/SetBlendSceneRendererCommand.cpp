#include "SceneRenderComands\SetBlendSceneRendererCommand.h"
#include "GraphicsManager.h"

CSetBlendSceneRendererCommand::CSetBlendSceneRendererCommand(
    CXMLTreeNode& atts ): CSceneRendererCommand( atts )
{
    if ( atts.Exists() )
    {
        m_SrcBlend =  atts.GetAttribute<std::string>( "srcblend", "" );
        m_DstBlend =  atts.GetAttribute<std::string>( "dstblend", "" );
		m_BlendOP = atts.GetAttribute<std::string>( "blendop", "" );
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