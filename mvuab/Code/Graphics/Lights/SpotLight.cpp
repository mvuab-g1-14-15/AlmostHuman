#include "SpotLight.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"

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
  , m_FallOff( node.GetFloatProperty( "fall_off", 0 ) )
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
  D3DXMatrixTranslation( &translation, eye.x , eye.y , eye.z );
  GraphicsInstance->SetTransform( matrix );
  GraphicsInstance->DrawSphere( 0.5f, Math::colMAGENTA );
  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  GraphicsInstance->SetTransform( matrix );
  GraphicsInstance->DrawLine( m_Position, m_Position + m_Direction.GetNormalized() * 5.0f,
                              Math::colMAGENTA );
}

void CSpotLight::SetShadowMap( CGraphicsManager* GM )
{
  // Calculate the View Matrix of the light
  D3DXVECTOR3 d3dxPos( m_Position.x, m_Position.y, m_Position.z );
  Math::Vect3f& l_LookAt = m_Position + m_Direction.GetNormalized();
  D3DXVECTOR3 d3dxTarget( l_LookAt.x, l_LookAt.y, l_LookAt.z );
  // Create the up vector with the ideal vector up of the word
  Math::Vect3f l_UpVec( 0.0f, 1.0f, 0.0f );
  const Math::Vect3f& l_SideVec = m_Direction.GetNormalized().CrossProduct( l_UpVec );
  l_UpVec = m_Direction.CrossProduct( l_SideVec );
  D3DXVECTOR3 d3dxUp( l_UpVec.x, l_UpVec.y, l_UpVec.z );
  D3DXMATRIX l_ViewD3DX;
  D3DXMatrixLookAtLH( &l_ViewD3DX, &d3dxPos, &d3dxTarget, &d3dxUp );
  m_ViewShadowMap = Math::Mat44f( l_ViewD3DX );
  D3DXMATRIX l_PerspectiveD3DX;
  D3DXMatrixPerspectiveFovLH( &l_PerspectiveD3DX, m_FallOff, 1.0f, 1.0f,
                              m_EndRangeAttenuation );
  m_ProjectionShadowMap = Math::Mat44f( l_PerspectiveD3DX );
  CEffectManager* l_EffectManager = CEffectManager::GetSingletonPtr();
  l_EffectManager->ActivateCamera( m_ViewShadowMap, m_ProjectionShadowMap, m_Position );
}