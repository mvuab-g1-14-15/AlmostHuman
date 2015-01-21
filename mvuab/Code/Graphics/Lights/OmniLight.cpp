#include "OmniLight.h"

#include "XML/XMLTreeNode.h"
#include "GraphicsManager.h"

COmniLight::COmniLight( CXMLTreeNode node ) : CLight( node )
{
  SetType( CLight::OMNI );
}

void COmniLight::Render()
{
  LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();

  D3DXMATRIX matrix = GetTransform().GetD3DXMatrix();

  l_Device->SetTransform( D3DTS_WORLD, &matrix );

  GraphicsInstance->DrawSphere( 0.5f, Math::colRED );

  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  l_Device->SetTransform( D3DTS_WORLD, &matrix );
}