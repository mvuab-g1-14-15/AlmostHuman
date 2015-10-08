#include "CameraCinematical.h"
#include "Timer\Timer.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "CameraKeyController.h"
#include "Utils/BaseUtils.h"

CCameraCinematical::CCameraCinematical( CCameraKeyController *KeyController)
    : CCamera()
    , m_pKeyController(KeyController)
{
    SetCameraType(CCamera::Cinematical);
}

CCameraCinematical::~CCameraCinematical()
{
    CHECKED_DELETE(m_pKeyController)
}

void CCameraCinematical::Update( )
{
	if( m_Enable )
	{
		m_pKeyController->Update();

		CCameraInfo * l_pCameraInfo = m_pKeyController->GetCurrentInfo();
		m_Position					= l_pCameraInfo->GetEye();
		m_LookAt					= l_pCameraInfo->GetLookAt();
		m_Up						= l_pCameraInfo->GetUp();
		m_ZFar						= l_pCameraInfo->GetFarPlane();
		m_ZNear						= l_pCameraInfo->GetNearPlane();
		m_FovInRadians				= l_pCameraInfo->GetFOV();
	}
	else
	{
		m_pKeyController->SetCurrentTime(0);
	}

#ifdef _DEBUG
    // The yaw is only calculated to draw the cinematic cameras frustum
    Math::Vect3f d = m_LookAt- m_Position;
    m_fYaw = Math::Utils::ATan2(d.z, d.x);
    m_fPitch = Math::Utils::ATan2(d.y,Math::Utils::Sqrt(d.z * d.z + d.x * d.x));
    //baseUtils::TraceVect3f("CameraPosition",m_Pos);
    // baseUtils::TraceVect3f("CameraLookAt",GetLookAt());
#endif
}

Math::Vect3f CCameraCinematical::GetLookAt(void) const
{
    return m_LookAt;
}

Math::Vect3f CCameraCinematical::GetVecUp(void) const
{
    return m_Up;
}