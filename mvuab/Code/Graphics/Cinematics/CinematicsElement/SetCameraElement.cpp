#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cameras\CameraManager.h"

CSetCameraElement::CSetCameraElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_sCamera( atts.GetPszProperty( "name", "Test_Process" ) )
	, m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f(0,0,0) ) )
{
}

void CSetCameraElement::Execute( CGraphicsManager& GM )
{
	CameraMInstance->SetCurrentCamera(m_sCamera);
	CameraMInstance->GetCamera(m_sCamera)->SetPosition(m_Position);
}

void CSetCameraElement::Render(CGraphicsManager &GM)
{
}

void CSetCameraElement::Render2D(CGraphicsManager &GM)
{
}