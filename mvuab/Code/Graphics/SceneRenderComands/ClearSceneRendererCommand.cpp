#include "SceneRenderComands\ClearSceneRendererCommand.h"
#include "GraphicsManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand( CXMLTreeNode& atts )
    : CSceneRendererCommand( atts )
    , m_Color( atts.GetAttribute<bool>( "color", false ) )
    , m_Depht( atts.GetAttribute<bool>( "depth", false ) )
    , m_Stencil( atts.GetAttribute<bool>( "stencil", false ) )
    , m_ClearColor( atts.GetAttribute<Math::CColor>("clearColor", Math::CColor(0, 0, 0)) )
{
}

void CClearSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CClearSceneRendererCommand::Execute", Profiler::Color::Orchid )
    GM.Clear( m_Color, m_Depht, m_Stencil, D3DCOLOR_XRGB((int)m_ClearColor.r, (int)m_ClearColor.g, (int)m_ClearColor.b) );
}