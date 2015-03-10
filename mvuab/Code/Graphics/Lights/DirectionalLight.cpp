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
  m_OrthoShadowMapSize.x = node.GetIntProperty( "shadow_map_width", 0 );
  m_OrthoShadowMapSize.y = node.GetIntProperty( "shadow_map_height", 0 );
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
	D3DXMATRIX l_View;
	D3DXMATRIX l_Ortho;

	D3DXVECTOR3 l_Eye(m_Position.x, m_Position.y, m_Position.z);
	m_Direction.Normalize();
	Vect3f l_LookAtV3f = m_Position + m_Direction;

	D3DXVECTOR3 l_LookAt(l_LookAtV3f.x, l_LookAtV3f.y, l_LookAtV3f.z);
	D3DXVECTOR3 l_VUP;

	float l_Value = v3fY*l_LookAtV3f;
	if ((v3fY*m_Direction)> 0.995f || (v3fNEGY*m_Direction)>0.995f)
	{
			Vect3f l_Cross = v3fX ^ m_Direction;
			Vect3f l_VUpV3f = m_Direction ^ l_Cross;
			l_VUP = D3DXVECTOR3(l_VUpV3f.x, l_VUpV3f.y, l_VUpV3f.z);
	}
	else
	{
			Vect3f l_Cross = v3fY ^ m_Direction;
			Vect3f l_VUpV3f = m_Direction ^ l_Cross;
			l_VUP = D3DXVECTOR3(l_VUpV3f.x, l_VUpV3f.y, l_VUpV3f.z);
	}
	//l_VUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&l_View, &l_Eye, &l_LookAt, &l_VUP);
	m_ViewShadowMap = Mat44f(l_View);
	D3DXMatrixOrthoLH(&l_Ortho, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.1f, m_EndRangeAttenuation);
	m_ProjectionShadowMap = Mat44f(l_Ortho);

	CEffectManager *l_EffectManager = CEffectManager::GetSingletonPtr();
	l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);

	LPDIRECT3DDEVICE9 lDevice = CGraphicsManager::GetSingletonPtr()->GetDevice();
	lDevice->SetTransform( D3DTS_VIEW, &l_View );
	lDevice->SetTransform( D3DTS_PROJECTION, &l_Ortho );
}
