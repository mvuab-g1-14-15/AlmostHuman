#include "Cinematics\CinematicsElement\SetTransformElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "Timer\Timer.h"
#include "Actor\PhysicActor.h"

#include "RenderableObject/Scene.h"
#include "RenderableObject/Room.h"

CSetTransformElement::CSetTransformElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_Position( atts.GetAttribute<Math::Vect3f>( "pos", Math::Vect3f( 0, 0, 0 ) ) )
	, m_pObject(SceneInstance->GetResource( atts.GetAttribute<std::string>("room", "") )->GetLayer(atts.GetAttribute<std::string>("layer", "solid") )->GetResource( atts.GetAttribute<std::string>( "name", "" ) ) )
    , m_Size( atts.GetAttribute<Math::Vect3f>( "size", Math::Vect3f( 0, 0, 0 ) ) )
    , m_Yaw( atts.GetAttribute<float>( "yaw", 0.0 ) )
    , m_Pitch( atts.GetAttribute<float>( "pitch", 0.0 ) )
    , m_Roll( atts.GetAttribute<float>( "roll", 0.0 ) )
{
}

void CSetTransformElement::Update()
{
	//ASSERT(m_pObject, "Error al leer el renderable en SetTransform");
	if(m_pObject == 0)
		m_pObject = SceneInstance->GetResource( "core" )->GetLayer( "characters" )->GetResource(m_Name );
    m_pObject->SetPosition( m_Position );
    m_pObject->SetScale( m_Size );
    m_pObject->SetYaw( m_Yaw );
    m_pObject->SetPitch( m_Pitch );
    m_pObject->SetRoll( m_Roll );
    m_pObject->MakeTransform();
    m_CurrentTime += deltaTimeMacro;

}
void CSetTransformElement::Execute( CGraphicsManager& GM )
{
}

void CSetTransformElement::Render()
{
}

void CSetTransformElement::Render2D( CGraphicsManager& GM )
{
}