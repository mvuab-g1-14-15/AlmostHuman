#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"
#include "Cinematics\CinematicManager.h"

CSetCameraElement::CSetCameraElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_CameraName(atts.GetAttribute<std::string>("name_camera", "") )
	, m_CameraCinematical( CameraMInstance->GetCamera( atts.GetAttribute<std::string>("name_camera", "")) )
	
{
}

void CSetCameraElement::Update()
{
	if(m_Visible)
	{
		CinematicMInstance->SetCurrentCamera( CameraMInstance->GetCurrentCameraName() );
		CameraMInstance->SetCurrentCamera( m_CameraName );
		CameraMInstance->GetCurrentCamera()->SetEnable(true);
		CinematicMInstance->SetCinematicActive( true );
	}
	else
	{
		CameraMInstance->GetCurrentCamera()->SetEnable(false);
		CameraMInstance->SetCurrentCamera( CinematicMInstance->GetCurrentCamera() );
		CinematicMInstance->SetCinematicActive( false );
	}
    m_CurrentTime += deltaTimeMacro;
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