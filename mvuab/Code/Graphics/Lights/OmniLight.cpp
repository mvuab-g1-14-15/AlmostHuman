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
  LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
  D3DXMATRIX matrix;
  D3DXMatrixTranslation( &matrix, m_Position.x, m_Position.y, m_Position.z );
  GraphicsInstance->SetTransform( matrix );
  GraphicsInstance->DrawSphere( 0.5f, Math::colRED );
  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  GraphicsInstance->SetTransform( matrix );
}

void COmniLight::SetShadowMap( CGraphicsManager* GM )
{
}