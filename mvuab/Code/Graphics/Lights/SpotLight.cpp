#include "SpotLight.h"
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

CSpotLight::CSpotLight( CXMLTreeNode node )
  : CDirectionalLight( node )
  , m_Angle( node.GetFloatProperty( "angle", 0 ) )
  , m_FallOff( node.GetFloatProperty( "falloff", 0 ) )
{
  SetType( CLight::SPOT );
}

void CSpotLight::SetFallOff( const float FallOff )
{
  m_FallOff = FallOff;
}

void CSpotLight::SetAngle( const float Angle )
{
  m_Angle = Angle;
}

float CSpotLight::GetAngle() const
{
  return m_Angle;
}

float CSpotLight::GetFallOff() const
{
  return m_FallOff;
}

void CSpotLight::Render()
{
  LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();

  D3DXMATRIX matrix = GetTransform().GetD3DXMatrix();
  D3DXMATRIX translation;
  D3DXVECTOR3 eye( m_Position.x, m_Position.y, m_Position.z );
  D3DXMatrixTranslation( &translation, eye.x ,eye.y ,eye.z );
                        
  D3DXMATRIX rotation;
  D3DXMATRIX rotation2;
      
  D3DXMatrixRotationY ( &rotation,  -m_fYaw + Math::half_pi32);
  D3DXMatrixRotationZ ( &rotation2, m_fPitch + Math::pi32);

  matrix = rotation2 * rotation * translation;

  l_Device->SetTransform( D3DTS_WORLD, &matrix );
  Math::Vect3f l_Direction = m_Direction.GetNormalized() * 4.0f;
  GraphicsInstance->DrawSphere( 0.5f, Math::colRED );
  GraphicsInstance->DrawCylinder( 0.5f, 0.0f, l_Direction.Length(), 30, Math::colYELLOW, true );

  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  l_Device->SetTransform( D3DTS_WORLD, &matrix );
}