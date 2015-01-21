#include "DirectionalLight.h"
#include "Math\Vector3.h"
#include "GraphicsManager.h"

#define D3DFVF_CUSTOMVERTEXLIGHT (D3DFVF_XYZ|D3DFVF_DIFFUSE)
typedef struct CUSTOMVERTEXLIGHT
{
  float x, y, z;
  DWORD color;

  static unsigned int getFlags()
  {
    return ( D3DFVF_CUSTOMVERTEXLIGHT );
  }
} CUSTOMVERTEXLIGHT;

CDirectionalLight::CDirectionalLight( const CXMLTreeNode& node )
  : CLight( node )
  , m_Direction( node.GetVect3fProperty( "dir", Math::Vect3f( 0, 0, 0 ) ) )
{
  m_fYaw = Math::Utils::ATan2( m_Direction.z, m_Direction.x );
  m_fPitch = Math::Utils::ATan2( m_Direction.y,
                                 Math::Utils::Sqrt( m_Direction.z * m_Direction.z + m_Direction.x * m_Direction.x ) );
  SetType( CLight::DIRECTIONAL );
}

void CDirectionalLight::SetDirection( const Math::Vect3f& Direction )
{
  m_Direction = Direction;
}

Math::Vect3f CDirectionalLight::GetDirection() const
{
  return m_Direction;
}

void CDirectionalLight::Render()
{
  LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();

  D3DXMATRIX matrix = GetTransform().GetD3DXMatrix();

  l_Device->SetTransform( D3DTS_WORLD, &matrix );
  Math::Vect3f l_Direction = m_Direction.GetNormalized() * 10.0f;
  CUSTOMVERTEXLIGHT v[2] =
  {
    {
      m_Position.x,
      m_Position.y,
      m_Position.z,
      0xff000000
    },
    {
      m_Position.x + l_Direction.x,
      m_Position.y + l_Direction.y,
      m_Position.z + l_Direction.z,
      0xff000000
    },
  };

  l_Device->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEXLIGHT ) );

  GraphicsInstance->DrawSphere( 0.5f, Math::colRED );

  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  l_Device->SetTransform( D3DTS_WORLD, &matrix );
}
