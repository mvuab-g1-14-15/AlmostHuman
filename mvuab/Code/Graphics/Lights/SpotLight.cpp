#include "SpotLight.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Effects\EffectManager.h"

#include "Cameras\Camera.h"
#include "Cameras\CameraManager.h"
#include "Billboard\BillboardCore.h"
#include "Billboard\BillboardInstance.h"
#include "Billboard/BillboardManager.h"


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

CSpotLight::CSpotLight()
  : CDirectionalLight()
  , m_Angle(0)
  , m_FallOff(0)
{
}

CSpotLight::CSpotLight( CXMLTreeNode node )
    : CDirectionalLight( node )
    , m_Angle( Math::Utils::Deg2Rad( node.GetAttribute<float>( "angle", 0 ) ))
    , m_FallOff( Math::Utils::Deg2Rad( node.GetAttribute<float>( "fall_off", 0 )))
{
    SetType( CLight::SPOT );
//     CBillboardCore* lBillboardCore = BillboardMan->GetResource("SpotLight");
//     if( lBillboardCore  )
//     {
//       mBillboard = new CBillboardInstance();
//       mBillboard->ChangePosition( m_Position );
//       lBillboardCore->AddInstance( mBillboard );
//     }
// 
//     lBillboardCore = BillboardMan->GetResource("LightTarget");
//     if( lBillboardCore  )
//     {
//       mLookAtBillboard = new CBillboardInstance();
//       mLookAtBillboard->ChangePosition( mLookAt );
//       lBillboardCore->AddInstance( mLookAtBillboard );
//     }
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
}

void CSpotLight::SetShadowMap( CGraphicsManager* GM )
{
    D3DXMATRIX l_View;
    D3DXMATRIX l_Projection;
    D3DXVECTOR3 l_Eye( m_Position.u );

    m_Direction.Normalize();
    Vect3f l_LookAtV3f = m_Position + m_Direction;

    D3DXVECTOR3 l_LookAt( l_LookAtV3f.u );
    D3DXVECTOR3 l_VUP;

    float l_Value = Math::v3fY * l_LookAtV3f;

    if ( ( Math::v3fY * m_Direction ) > 0.995f || ( Math::v3fNEGY * m_Direction ) > 0.995f )
    {
        Math::Vect3f l_Cross = v3fX ^ m_Direction;
        Math::Vect3f l_VUpV3f = m_Direction ^ l_Cross;
        l_VUP = D3DXVECTOR3( l_VUpV3f.u );
    }
    else
    {
        Math::Vect3f l_Cross = Math::v3fY ^ m_Direction;
        Math::Vect3f l_VUpV3f = m_Direction ^ l_Cross;
        l_VUP = D3DXVECTOR3( l_VUpV3f.u );
    }

    l_VUP.x = 0.0f;
    l_VUP.y = 1.0f;
    l_VUP.z = 0.0f;

    D3DXVec3Normalize( &l_VUP, &l_VUP );
    D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP );
    m_ViewShadowMap = Math::Mat44f( l_View );

    D3DXMatrixPerspectiveFovLH( &l_Projection, m_FallOff, 1.0f, 1.0f, m_EndRangeAttenuation );
    m_ProjectionShadowMap = Math::Mat44f( l_Projection );

    CEffectManager* l_EffectManager = EffectManagerInstance;
    l_EffectManager->ActivateCamera( m_ViewShadowMap, m_ProjectionShadowMap, m_Position );

    CEngineManagers::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->UpdateFrustum(l_View * l_Projection);
}