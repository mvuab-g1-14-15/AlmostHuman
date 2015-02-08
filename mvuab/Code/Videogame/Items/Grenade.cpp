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
  , m_GrenadeThrown( false )
  , m_ElapsedTime( 0.0f )
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
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
  if( CActionManager::GetSingletonPtr()->DoAction("ThrowGrenade") )
  {
    CCamera * l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();
    if( l_CurrentCamera && !m_GrenadeThrown )
    {
      CPhysicUserData* l_PhysicUserData = new CPhysicUserData("Grenade");
      l_PhysicUserData->SetPaint(true);
      m_Actor = new CPhysicActor(l_PhysicUserData);
      m_Actor->AddSphereShape(0.1f,l_CurrentCamera->GetPos(),Math::Vect3f(0,0,0));
      m_Actor->CreateBody(1.0f);

      // Add at the end always it needs to have a shape
      l_PM->AddPhysicActor(m_Actor);
      m_Actor->SetLinearVelocity( l_CurrentCamera->GetDirection().GetNormalized() * m_Force );

      m_GrenadeThrown = true;
    }
  }

  if( m_GrenadeThrown )
  {
    m_ElapsedTime += deltaTime;

    if( m_ElapsedTime > m_TimeAux )
    {
      std::vector<CPhysicUserData*> l_ObjectsAffectedByGranade;

      Math::Vect3f l_GrenadePosition = m_Actor->GetPosition();
      const std::set<CPhysicUserData*> &l_ImpactedObjects = l_PM->OverlapSphere( 10.f, m_Actor->GetPosition(), DYNAMIC_SHAPE );

      std::set<CPhysicUserData*>::const_iterator itb(l_ImpactedObjects.begin()), ite(l_ImpactedObjects.end());
      for(; itb != ite ; ++itb )
      {
        CPhysicUserData* l_Data = *itb;
        CPhysicActor* l_Actor = l_Data->GetActor();
        l_Actor->SetLinearVelocity( ( l_Actor->GetPosition() - l_GrenadePosition ) * 10.f );
      }

      // To throw a new grenade
      m_ElapsedTime = 0.0f;
      m_GrenadeThrown = false;
    }
  }
}

void CGrenade::Render()
{
  // Why render if no grenade
  if( !m_GrenadeThrown )
    return;

  // Get the graphics manager
  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();

  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();

  l_PM->DebugRender( l_GM );

  // Set the identity
  Math::Mat44f t;
  CGraphicsManager::GetSingletonPtr()->SetTransform( t );
}