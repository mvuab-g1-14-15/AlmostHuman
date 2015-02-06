#include "Grenade.h"

#include "Core.h"
#include "Timer\Timer.h"
#include "StaticMeshes\StaticMesh.h"
#include "Actor\PhysicActor.h"
#include "PhysicsManager.h"
#include "Utils\PhysicUserData.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "GraphicsManager.h"
#include "ActionManager.h"
#include "Cameras\CameraManager.h"
#include "Cameras\Camera.h"

#include <string>

CGrenade::CGrenade(float a_TimeAux, float a_ForceMin, float a_ForceMax, float a_Force, std::string a_MeshName)
	: m_TimeAux( a_TimeAux )
	, m_ForceMin( a_ForceMin )
	, m_ForceMax( a_ForceMax )
	, m_Force( a_Force )
	, m_MeshName( a_MeshName )
	, _m_throw( false )
{
}

CGrenade::~CGrenade()
{
	CHECKED_DELETE( m_Actor );
}

void CGrenade::Start()
{
}

void CGrenade::Update()
{
	if( CActionManager::GetSingletonPtr()->DoAction("ThrowGrenade") )
	{
			CCamera * l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();
			if( l_CurrentCamera )
			{
				CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
				CPhysicUserData* l_PhysicUserData = new CPhysicUserData("Grenade");
				l_PhysicUserData->SetPaint(true);
				m_Actor = new CPhysicActor(l_PhysicUserData);
				m_Actor->AddSphereShape(0.1f,l_CurrentCamera->GetPos(),Math::Vect3f(0,0,0));
				m_Actor->CreateBody(1.0f);
				
				// Add at the end allways it needs to have a shape
				l_PM->AddPhysicActor(m_Actor);				
				m_Actor->SetLinearVelocity( l_CurrentCamera->GetDirection().GetNormalized() * m_Force );
			}
	}
   
}

void CGrenade::Render()
{
	// Why render if no grenade
	if( !_m_throw ) 
		return;

	// Get the graphics manager
	CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();

	CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();

	l_PM->DebugRender( l_GM );

	// Set the the transform
	//l_GM->SetTransform(m_Actor->GetMat44());

	// Render with that transform the static mesh
	//m_StaticMesh->Render(l_GM);
	//l_GM->DrawSphere( 1.0f );

	// Set the identity
	Math::Mat44f t;
	CGraphicsManager::GetSingletonPtr()->SetTransform( t );
}