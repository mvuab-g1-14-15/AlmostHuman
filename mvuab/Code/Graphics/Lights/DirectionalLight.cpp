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
  m_fYaw = -Math::Utils::ATan2( m_Direction.z, m_Direction.x );
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
  D3DXMATRIX translation;
  D3DXVECTOR3 eye( m_Position.x, m_Position.y, m_Position.z );
  D3DXMatrixTranslation( &translation, eye.x ,eye.y ,eye.z );

  D3DXMATRIX rotation;
  D3DXMATRIX rotation2;

  D3DXMatrixRotationY ( &rotation,  -m_fYaw + Math::half_pi32);
  D3DXMatrixRotationZ ( &rotation2, m_fPitch);

  matrix = rotation2 * rotation * translation;

  GraphicsInstance->SetTransform(matrix );
  Math::Vect3f l_Direction = m_Direction.GetNormalized() * 5.0f;
  GraphicsInstance->DrawSphere( 0.5f, Math::colRED );
  GraphicsInstance->DrawCylinder( 0.2f, 0.2f, l_Direction.Length(), 30, Math::colYELLOW, true );

  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  GraphicsInstance->SetTransform( matrix );
}
