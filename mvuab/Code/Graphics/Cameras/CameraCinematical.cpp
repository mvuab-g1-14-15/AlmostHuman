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
    m_Position     = l_pCameraInfo->GetEye();
    m_LookAt       = l_pCameraInfo->GetLookAt();
    m_Up           = l_pCameraInfo->GetUp();
    m_ZFar         = l_pCameraInfo->GetFarPlane();
    m_ZNear        = l_pCameraInfo->GetNearPlane();
    m_FovInRadians =  l_pCameraInfo->GetFOV();
    MakeTransform();
  }
  else
  {
    m_pKeyController->SetCurrentTime(0);
  }
}

Math::Vect3f CCameraCinematical::GetLookAt(void) const
{
  return m_LookAt;
}

Math::Vect3f CCameraCinematical::GetVecUp(void) const
{
  return m_Up;
}