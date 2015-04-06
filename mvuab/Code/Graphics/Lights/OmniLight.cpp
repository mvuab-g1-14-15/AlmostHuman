#include "OmniLight.h"

#include "XML/XMLTreeNode.h"
#include "GraphicsManager.h"

COmniLight::COmniLight( CXMLTreeNode node ) : CLight( node )
{
  SetType( CLight::OMNI );
}

COmniLight::COmniLight( ) : CLight( )
{
  SetType( CLight::OMNI );
}

void COmniLight::Render()
{
  GraphicsInstance->DrawSphere( m_Position, 0.2f, Math::colRED );
}

void COmniLight::SetShadowMap( CGraphicsManager* GM )
{
}