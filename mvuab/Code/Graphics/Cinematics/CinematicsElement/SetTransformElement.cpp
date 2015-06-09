#include "Cinematics\CinematicsElement\SetTransformElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "Timer\Timer.h"
#include "Actor\PhysicActor.h"

CSetTransformElement::CSetTransformElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f(0,0,0) ) )
  , m_pObject( dynamic_cast<CInstanceMesh*>(ROLMInstance->GetResource("solid")->GetResource( atts.GetPszProperty("object", "no_object") ) ) )
  , m_Size( atts.GetVect3fProperty( "size", Math::Vect3f(0,0,0) ) )
  , m_Yaw( atts.GetFloatProperty( "yaw", 0.0 ) )
  , m_Pitch( atts.GetFloatProperty( "pitch", 0.0 ) )
  , m_Roll( atts.GetFloatProperty( "roll", 0.0 ) )
{
}

void CSetTransformElement::Update()
{
  m_pObject->SetPosition(m_Position);
  m_pObject->SetScale(m_Size);
  m_pObject->SetYaw(m_Yaw);
  m_pObject->SetPitch(m_Pitch);
  m_pObject->SetRoll(m_Roll);
  m_pObject->MakeTransform();
  m_pActorObject = m_pObject->GetActor();
  m_pActorObject->SetGlobalPosition(m_Position);
  m_CurrentTime+= deltaTimeMacro;

}
void CSetTransformElement::Execute( CGraphicsManager& GM )
{
}

void CSetTransformElement::Render()
{
}

void CSetTransformElement::Render2D(CGraphicsManager &GM)
{
}