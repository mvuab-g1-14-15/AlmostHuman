#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"

CSetCameraElement::CSetCameraElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f(0, 0, 0) ) )
    , m_Yaw( atts.GetAttribute<float>( "yaw", 0.0 ) )
    , m_Pitch( atts.GetAttribute<float>( "pitch", 0.0 ) )
    , m_Roll( atts.GetAttribute<float>( "roll", 0.0 ) )
{
}

void CSetCameraElement::Update()
{
    CCamera* lCamera = CameraMInstance->GetCurrentCamera();
    lCamera->SetPosition(m_Position);
    lCamera->SetYaw(m_Yaw);
    lCamera->SetPitch(m_Pitch);
    lCamera->SetRoll(m_Roll);
    lCamera->MakeTransform();
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