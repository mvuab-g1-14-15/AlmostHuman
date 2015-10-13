#include "CameraCinematical.h"
#include "Timer\Timer.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "CameraKeyController.h"
#include "Utils/BaseUtils.h"
#include "Timer/CounTDownTimerManager.h"
#include "EngineManagers.h"
#include "Utils\StringUtils.h"
#include "Utils\Defines.h"

bool timer = false;
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
		//#ifdef _DEBUG
		// The yaw is only calculated to draw the cinematic cameras frustum
		Math::Vect3f d = m_LookAt- m_Position;
		m_fYaw = Math::Utils::ATan2(d.z, d.x);
		m_fPitch = Math::Utils::ATan2(d.y,Math::Utils::Sqrt(d.z * d.z + d.x * d.x));
		MakeTransform();
		//m_fRoll = 
		//baseUtils::TraceVect3f("CameraPosition",m_Pos);
		// baseUtils::TraceVect3f("CameraLookAt",GetLookAt());
		//#endif
	}
	else
	{
		m_pKeyController->SetCurrentTime(0);
	}


}

Math::Vect3f CCameraCinematical::GetLookAt(void) const
{
    //return m_LookAt;
	Math::Vect3f look( Math::Utils::Cos(m_fYaw) * Math::Utils::Cos(m_fPitch),
                       Math::Utils::Sin(m_fPitch),
                       Math::Utils::Sin(m_fYaw) * Math::Utils::Cos(m_fPitch) );

    return m_Position + look;
}

Math::Vect3f CCameraCinematical::GetVecUp(void) const
{
    return m_Up;
	Math::Vect3f vUpVec( -Math::Utils::Cos(m_fYaw) * Math::Utils::Sin(m_fPitch),
                         Math::Utils::Cos(m_fPitch),
                         -Math::Utils::Sin(m_fYaw) * Math::Utils::Sin(m_fPitch) );
	return vUpVec;
}