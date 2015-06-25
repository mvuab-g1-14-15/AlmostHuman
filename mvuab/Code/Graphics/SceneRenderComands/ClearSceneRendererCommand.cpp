#include "SceneRenderComands\ClearSceneRendererCommand.h"
#include "GraphicsManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand( CXMLTreeNode& atts )
    : CSceneRendererCommand( atts )
    , m_Color( atts.GetAttribute<bool>( "color", false ) )
    , m_Depht( atts.GetAttribute<bool>( "depth", false ) )
    , m_Stencil( atts.GetAttribute<bool>( "stencil", false ) )
{
}

void CClearSceneRendererCommand::Execute( CGraphicsManager& GM )
{
    GM.Clear( m_Color, m_Depht, m_Stencil, D3DCOLOR_XRGB( 0, 0, 0 ) );
}