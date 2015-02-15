#include "DirectionalLight.h"
#include "Math\Vector3.h"
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

CDirectionalLight::CDirectionalLight( const CXMLTreeNode& node )
  : CLight( node )
  , m_Direction( node.GetVect3fProperty( "dir", Math::Vect3f( 0, 0, 0 ) ) )
{
  Math::Vect3f l_Direction = m_Direction.GetNormalized();
  m_fYaw = -Math::Utils::ATan2( l_Direction.z, l_Direction.x );
  m_fPitch = Math::Utils::ATan2( l_Direction.y,
                                 Math::Utils::Sqrt( l_Direction.z * l_Direction.z + l_Direction.x *
                                     l_Direction.x ) );
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
  D3DXMatrixTranslation( &translation, eye.x , eye.y , eye.z );
  GraphicsInstance->SetTransform( matrix );
  GraphicsInstance->DrawSphere( 0.5f, Math::colGREEN );
  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  GraphicsInstance->SetTransform( matrix );
  GraphicsInstance->DrawLine( m_Position, m_Position + m_Direction.GetNormalized() * 5.0f,
                              Math::colGREEN );
}

void CDirectionalLight::SetShadowMap( CGraphicsManager* GM )
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
  D3DXMATRIX l_OrthoD3DX;
  D3DXMatrixOrthoLH( &l_OrthoD3DX, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 1.0f,
                     m_EndRangeAttenuation );
  m_ProjectionShadowMap = Math::Mat44f( l_OrthoD3DX );
  CEffectManager* l_EffectManager = CEffectManager::GetSingletonPtr();
  l_EffectManager->ActivateCamera( m_ViewShadowMap, m_ProjectionShadowMap, m_Position );
}
