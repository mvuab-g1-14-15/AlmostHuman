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
#include "EngineManagers.h"

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
   for ( size_t i = 0; i < m_vPhysicUserData.size(); ++i )
    CHECKED_DELETE( m_vPhysicUserData[i] );

  m_vPhysicUserData.clear();

  for ( size_t i = 0; i < m_vActor.size(); ++i )
    CHECKED_DELETE( m_vActor[i] );

  m_vActor.clear();
}

void CGrenade::Start()
{
}

void CGrenade::Update()
{
  CPhysicsManager* l_PM = PhysXMInstance;
  if( ActionManagerInstance->DoAction("ThrowGrenade") )
  {
    CCamera * l_CurrentCamera = CameraMInstance->GetCurrentCamera();
    if( l_CurrentCamera && !m_GrenadeThrown )
    {
      m_PhysicUserData = new CPhysicUserData("Grenade");
      m_PhysicUserData->SetPaint(true);
      m_vPhysicUserData.push_back(m_PhysicUserData);
      m_Actor = new CPhysicActor(m_PhysicUserData);
      m_Actor->AddSphereShape(0.1f,l_CurrentCamera->GetPosition(),Math::Vect3f(0,0,0));
      m_Actor->CreateBody(1.0f);
      m_vActor.push_back(m_Actor);

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
        if(l_Data->GetActor() != NULL)
        {
          CPhysicActor* l_Actor = l_Data->GetActor();
          l_Actor->SetLinearVelocity( ( l_Actor->GetPosition() - l_GrenadePosition ) * 10.f );
        }
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


  PhysXMInstance->Render();

  // Set the identity
  Math::Mat44f t;
  GraphicsInstance->SetTransform( t );
}