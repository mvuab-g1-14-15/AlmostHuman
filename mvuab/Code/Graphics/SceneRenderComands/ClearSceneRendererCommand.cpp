#include "SceneRenderComands\ClearSceneRendererCommand.h"
#include "GraphicsManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
  , m_Color( atts.GetBoolProperty( "color", false ) )
  , m_Depht( atts.GetBoolProperty( "depth", false ) )
  , m_Stencil( atts.GetBoolProperty( "stencil", false ) )
{
}

void CClearSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  GM.Clear( m_Color, m_Depht, m_Stencil, 0xffffffff );
}