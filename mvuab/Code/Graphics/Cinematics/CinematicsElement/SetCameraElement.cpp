#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cameras\CameraManager.h"

CSetCameraElement::CSetCameraElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f(0,0,0) ) )
  , m_Yaw( atts.GetFloatProperty( "yaw", 0.0 ) )
  , m_Pitch( atts.GetFloatProperty( "pitch", 0.0 ) )
  , m_Roll( atts.GetFloatProperty( "roll", 0.0 ) )
{
}

void CSetCameraElement::Execute( CGraphicsManager& GM )
{
  CCamera* lCamera = CameraMInstance->GetCurrentCamera();
  lCamera->SetPosition(m_Position);
  lCamera->SetYaw(m_Yaw);
  lCamera->SetPitch(m_Pitch);
  lCamera->SetRoll(m_Roll);
}

void CSetCameraElement::Render(CGraphicsManager &GM)
{
}

void CSetCameraElement::Render2D(CGraphicsManager &GM)
{
}