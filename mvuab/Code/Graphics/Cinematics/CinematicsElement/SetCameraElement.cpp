#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"
#include "Cinematics\CinematicManager.h"
#include "ScriptManager.h"
#include <sstream>

CSetCameraElement::CSetCameraElement( CXMLTreeNode& atts )
  : CCinematicsElement( atts )
  , m_CameraName(atts.GetAttribute<std::string>("name_camera", "") )
  , m_CameraCinematical( CameraMInstance->GetCamera( atts.GetAttribute<std::string>("name_camera", "")) )
  , m_CurrentCameraName( atts.GetAttribute<std::string>("name_current_camera", "") )
{
}

void CSetCameraElement::Update()
{
  CCameraManager* lCM      = CameraMInstance;
  CCinematicManager * lCiM = CinematicMInstance;
  if(m_Visible)
  {
    if(m_CurrentCameraName != "")
      lCiM->SetCurrentCamera( m_CurrentCameraName );
    else
      lCiM->SetCurrentCamera( lCM->GetCurrentCameraName() );
    lCM->SetCurrentCamera( m_CameraName );
    lCM->GetCurrentCamera()->SetEnable(true);
  }
  else
  {
    // Obtain the camera before playing the cinematic
    CCamera* lPreviousCamera = lCM->GetCamera( lCiM->GetCurrentCamera() );

    // Obtain the camera of the cinematic
    CCamera* lCurrentCamera  = lCM->GetCurrentCamera();

    // Change the position of the previous camera to be in the last position an looking at of the camera of the cinematic
    lPreviousCamera->SetPosition( lCurrentCamera->GetPosition() );
    lPreviousCamera->SetDirection( lCurrentCamera->GetDirection() );

    if( lCiM->GetCurrentCamera() == "Main" )
    {
      const Math::Vect3f& lPosition = lPreviousCamera->GetPosition();
      std::stringstream lCode;
      lCode << "g_Player:UpdatePlayer(Vect3f(" << lPosition.x << ", " << lPosition.y << ", " << lPosition.z << "), " << lPreviousCamera->GetYaw() << ", " << lPreviousCamera->GetPitch() << ")";
      ScriptMInstance->RunCode(lCode.str());
    }
    
    // Switch the cameras
    lPreviousCamera->SetEnable( true );
    lCurrentCamera->SetEnable( false );
    lCM->SetCurrentCamera( lCiM->GetCurrentCamera() );

  }

  m_CurrentTime += constFrameTime;
}
void CSetCameraElement::Execute( CGraphicsManager& GM )
{

}

void CSetCameraElement::Render()
{
}

void CSetCameraElement::Render2D(CGraphicsManager &GM)
{
}