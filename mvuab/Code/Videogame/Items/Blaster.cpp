#include "Blaster.h"

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

CBlaster::CBlaster( float a_TimeAux, float m_PressedTime, float a_Force, std::string a_MeshName )
  : m_TimeAux( a_TimeAux )
  , m_PressedTime( 0 )
  , m_Force( a_Force )
  , m_MeshName( a_MeshName )
  , m_BlasterShoot( false )
  , m_ElapsedTime( 0.0f )
{
}

CBlaster::~CBlaster()
{
  for ( size_t i = 0; i < m_vPhysicUserData.size(); ++i )
    CHECKED_DELETE( m_vPhysicUserData[i] );

  m_vPhysicUserData.clear();

  for ( size_t i = 0; i < m_vActor.size(); ++i )
    CHECKED_DELETE( m_vActor[i] );

  m_vActor.clear();
}

void CBlaster::Start()
{
}

void CBlaster::Update()
{
  CGraphicsManager* l_GM = GraphicsInstance;
  CPhysicsManager* l_PM = PhysXMInstance;

  CCamera* l_CurrentCamera = CameraMInstance->GetCurrentCamera();

  if ( l_CurrentCamera )
  {
    //CPhysicsManager* l_PM = PhysXMInstance;
    //CPhysicUserData* l_PhysicUserData = new CPhysicUserData( "Disparo" );
    //l_PhysicUserData->SetColor( colRED );
    //l_PhysicUserData->SetPaint( true );
    //m_vPhysicUserData.push_back( l_PhysicUserData );

    //CPhysicActor* l_Actor = new CPhysicActor( l_PhysicUserData );
    ////If don't want box, you can remove this line
    //CPhysicController* l_PC = l_PM->GetUserData( "CharacterController" )->GetController();
    //l_Actor->AddBoxShapeHardcoded( Math::Vect3f( 0.05f, 0.05f, 0.05f ),
    //                               l_PC->GetPosition(), Math::Vect3f( 0, 0, 0 ), Math::Vect3f( 0, 0, 0 ) );
    //l_Actor->CreateBody( 1.0f );
    //// Add at the end allways it needs to have a shape
    //l_PM->AddPhysicActor( l_Actor );
    //m_vActor.push_back( l_Actor );
    //Math::Vect3f l_Direction( l_CurrentCamera->GetDirection() );
    //l_Actor->SetLinearVelocity( l_Direction.GetNormalized() *
    //                            20.0f );
    //SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
    //uint32 mask = 1 << ECG_ESCENE;
    ////CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo);
    //CPhysicUserData* l_PUD = l_PM->RaycastClosestActorShoot(
    //                           l_PC->GetPosition(), l_Direction.GetNormalized(),
    //                           mask, l_SCollisionInfo, 40.0f );
    
    CPhysicController* l_PC = l_PM->GetUserData( "CharacterController" )->GetController();
    Math::Vect3f l_Direction( l_CurrentCamera->GetDirection() );
    SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
    uint32 mask = 1 << ECG_ESCENE;
    CPhysicUserData* l_PUD = l_PM->RaycastClosestActorShoot(
                               l_PC->GetPosition(), l_Direction.GetNormalized(),
                               mask, l_SCollisionInfo, 40.0f );

    Math::Vect3f l_CurrentPos = l_CurrentCamera->GetPosition();
    Math::Vect3f l_Stride = l_CurrentCamera->GetDirection().CrossProduct( l_CurrentCamera->GetVecUp() ).GetNormalized();
    Math::Vect3f l_Pos = l_CurrentPos - l_Stride*0.04f;
    l_Pos.y -= 0.02f;

    m_InitialPos = l_Pos;
    m_FinalPos = l_SCollisionInfo.m_CollisionPoint;

    /*l_PUD = new CPhysicUserData("Cubito");
    l_PUD->SetColor(Math::colBLACK);
    l_PUD->SetPaint(true);
    CPhysicActor* l_PA = new CPhysicActor(l_PUD);
    l_PA->AddBoxShape(Math::Vect3f(5,5,5), l_SCollisionInfo.m_CollisionPoint);
    l_PM->AddPhysicActor(l_PA);*/

    //l_GM->DrawLine(l_Pos, l_SCollisionInfo.m_CollisionPoint, Math::colORANGE );

    m_BlasterShoot = true;
  }

  // if ( ActionManagerInstance->DoAction( "LeftMouseButtonPressed" ) )
}

void CBlaster::Render()
{
  // Why render if no grenade
  if ( !m_BlasterShoot )
    return;

  
  PhysXMInstance->Render( );
  // Get the graphics manager
  
  CGraphicsManager* l_GM = GraphicsInstance;
  // Set the identity
  Math::Mat44f t;
  l_GM->SetTransform( t );

  l_GM->DrawLine(m_InitialPos, m_FinalPos, Math::colORANGE );
}