#include "CameraCinematical.h"
#include "Timer\Timer.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "CameraKeyController.h"
#include "Utils/BaseUtils.h"

CCameraCinematical::CCameraCinematical( CCameraKeyController *KeyController)
    : CCamera() , m_pKeyController(KeyController)
{
}

CCameraCinematical::~CCameraCinematical()
{
    CHECKED_DELETE(m_pKeyController)
}

void CCameraCinematical::Update( )
{
    m_pKeyController->Update();

    CCameraInfo * l_pCameraInfo = m_pKeyController->GetCurrentInfo();
    m_Pos = l_pCameraInfo->GetEye();
    m_LookAt = l_pCameraInfo->GetLookAt();
    m_Up = l_pCameraInfo->GetUp();
    m_ZFar = l_pCameraInfo->GetFarPlane();
    m_ZNear = l_pCameraInfo->GetNearPlane();
    m_fov_radians = l_pCameraInfo->GetFOV();
    baseUtils::TraceVect3f("CameraPosition",m_Pos);
    baseUtils::TraceVect3f("CameraLookAt",GetLookAt());
}

Math::Vect3f CCameraCinematical::GetLookAt(void) const
{
    return m_LookAt;
}

Math::Vect3f CCameraCinematical::GetVecUp(void) const
{
    return m_Up;
}