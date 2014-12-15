#include "TPSCamera.h"
#include "Math\Matrix33.h"
#include "Math\Matrix44.h"
#include <d3dx9.h>

CTPSCamera::CTPSCamera(Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio)
: CCamera( aEyePosition, aLookAt, aZNear, aZFar, aFOV, aAspectRatio,"TPS" )
{
    m_Zoom = m_EyePosition.Distance(m_LookAt);
}

CTPSCamera::CTPSCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject )
: CCamera( aEyePosition, aLookAt, apAttachedObject, "TPS" )
{
    m_Zoom = m_EyePosition.Distance(m_LookAt);
}

void CTPSCamera::RecalculateCameraData()
{
    Vect3f newVectDir( m_Zoom * mathUtils::Cos(m_Yaw) * mathUtils::Cos(m_Pitch),
                       m_Zoom * mathUtils::Sin(m_Pitch),
                       m_Zoom * mathUtils::Cos(m_Pitch) * mathUtils::Sin(m_Yaw) );

    m_EyePosition = m_LookAt + newVectDir; 
}

void CTPSCamera::AddZoom( float32 dz )
{
    m_Zoom += dz;
    if( m_Zoom > m_ZFar*0.8f )
    {
        m_Zoom = m_ZFar*0.8f;
    }
    else if( m_Zoom < m_ZNear*2.f)
    {
        m_Zoom = m_ZNear*2.f;
    }
}