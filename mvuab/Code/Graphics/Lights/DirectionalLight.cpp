#include "DirectionalLight.h"

#include "Math\Vector3.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Effects\EffectManager.h"

#include "Cameras\Camera.h"
#include "Cameras\CameraManager.h"

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
{
    const Math::Vect3f& lLookAt = node.GetVect3fProperty( "look_at", Math::Vect3f( 0, 0, 0 ) );
    m_Direction = (lLookAt - m_Position).GetNormalized();
    m_OrthoShadowMapSize.x = node.GetFloatProperty( "shadow_map_width", 0 );
    m_OrthoShadowMapSize.y = node.GetFloatProperty( "shadow_map_height", 0 );
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
    GraphicsInstance->DrawSphere( m_Position, 0.3f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.1f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.2f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.3f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.4f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.5f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.6f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.7f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.8f, 0.08f, Math::colGREEN );
    GraphicsInstance->DrawSphere( m_Position + m_Direction * 0.9f, 0.08f, Math::colGREEN );
}

void CDirectionalLight::SetShadowMap( CGraphicsManager* GM )
{
    D3DXMATRIX l_View;
    D3DXMATRIX l_Ortho;
    D3DXVECTOR3 l_Eye( m_Position.x, m_Position.y, m_Position.z );

    m_Direction.Normalize();
    Vect3f l_LookAtV3f = m_Position + m_Direction;

    D3DXVECTOR3 l_LookAt( l_LookAtV3f.x, l_LookAtV3f.y, l_LookAtV3f.z );
    D3DXVECTOR3 l_VUP;

    float l_Value = v3fY * l_LookAtV3f;

    if ( ( v3fY * m_Direction ) > 0.995f || ( v3fNEGY * m_Direction ) > 0.995f )
    {
        Vect3f l_Cross = v3fX ^ m_Direction;
        Vect3f l_VUpV3f = m_Direction ^ l_Cross;
        l_VUP = D3DXVECTOR3( l_VUpV3f.x, l_VUpV3f.y, l_VUpV3f.z );
    }
    else
    {
        Vect3f l_Cross = v3fY ^ m_Direction;
        Vect3f l_VUpV3f = m_Direction ^ l_Cross;
        l_VUP = D3DXVECTOR3( l_VUpV3f.x, l_VUpV3f.y, l_VUpV3f.z );
    }

    D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP );
    m_ViewShadowMap = Mat44f( l_View );

    D3DXMatrixOrthoLH( &l_Ortho, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.1f, m_EndRangeAttenuation );
    m_ProjectionShadowMap = Mat44f( l_Ortho );

    CEffectManager* l_EffectManager = EffectManagerInstance;
    l_EffectManager->ActivateCamera( m_ViewShadowMap, m_ProjectionShadowMap, m_Position );

    CEngineManagers::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->UpdateFrustum(l_View * l_Ortho);

    LPDIRECT3DDEVICE9 lDevice = GraphicsInstance->GetDevice();
    lDevice->SetTransform( D3DTS_VIEW, &l_View );
    lDevice->SetTransform( D3DTS_PROJECTION, &l_Ortho );
}