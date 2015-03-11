#include "PlayerPhysicProcess.h"
#include "Items\Grenade.h"
#include "Items\Blaster.h"

//AI
#include "Graph\Graph.h"
#include "Characters\Character.h"
#include "Pathfinding\AStar.h"

//BASE
#include "Logger\Logger.h"
#include "Utils\Defines.h"
#include "Fonts\FontManager.h"
#include "ScriptManager\ScriptManager.h"

//CORE
#include "Core.h"

//GRAPHICS
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

//INPUTS
#include "InputManager.h"
#include "ActionManager.h"

//PHYSX
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "Cooking Mesh\PhysicCookingMesh.h"

CPlayerPhysicProcess::CPlayerPhysicProcess() : CProcess()
{
}

CPlayerPhysicProcess::~CPlayerPhysicProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();
  CHECKED_DELETE( m_Grenade );
  CHECKED_DELETE( m_Blaster );

  for ( size_t i = 0; i < m_vPA.size(); ++i )
    CHECKED_DELETE( m_vPA[i] );

  m_vPA.clear();

  for ( size_t i = 0; i < m_vPUD.size(); ++i )
    CHECKED_DELETE( m_vPUD[i] );

  m_vPUD.clear();

  for ( size_t i = 0; i < m_vCharacter.size(); ++i )
    CHECKED_DELETE( m_vCharacter[i] );

  m_vCharacter.clear();
  CHECKED_DELETE( m_PhysicController );
  CHECKED_DELETE( m_pAStarScene );
}

void CPlayerPhysicProcess::Update()
{
  /*
  CGraph l_Graph;
  l_Graph.Parse("./Data/graph1.xml");

  unsigned int a = l_Graph.GetArcWeight(0, 3);
  unsigned int b = l_Graph.GetArcWeight(4, 1);

  Math::Vect3f v = l_Graph.GetNodeInfo(4);
  */

  /////////////////////////////////////////////////////////////
  ////////////      RELOADS ACTIONS           /////////////////
  /////////////////////////////////////////////////////////////
  CActionManager* pActionManager = CActionManager::GetSingletonPtr();

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    CCore::GetSingletonPtr()->GetStaticMeshManager()->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();

  if ( pActionManager->DoAction( "ReloadShaders" ) )
  {
    // NOTE this must be in this order
    CEffectManager::GetSingletonPtr()->Reload();
    CLightManager::GetSingletonPtr()->ReLoad();
    CRenderableObjectTechniqueManager::GetSingletonPtr()->ReLoad();
    CStaticMeshManager::GetSingletonPtr()->Reload();
    CRenderableObjectsLayersManager::GetSingletonPtr()->Reload();
    CSceneRendererCommandManager::GetSingletonPtr()->ReLoad();
  }

  if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    CCore::GetSingletonPtr()->GetActionManager()->Reload();


  ////////////////////////////////////////////////////////////////////
  ////////////        UPDATE CHARACTER CONTROLLER         ////////////
  ////////////////////////////////////////////////////////////////////

  /* Math::Vect3f l_Direction = Math::Vect3f( 0.0f, 0.0f, 0.0f );
   float  l_Yaw = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetYaw();

   if ( pActionManager->DoAction( "MoveForward" ) )
     l_Direction += Math::Vect3f( Math::Utils::Cos( l_Yaw ), 0.0f, Math::Utils::Sin( l_Yaw ) );

   if ( pActionManager->DoAction( "MoveBackward" ) )
     l_Direction -= Math::Vect3f( Math::Utils::Cos( l_Yaw ), 0.0f, Math::Utils::Sin( l_Yaw ) );

   if ( pActionManager->DoAction( "MoveLeft" ) )
     l_Direction += Math::Vect3f( Math::Utils::Cos( l_Yaw + Math::pi32 / 2 ), 0.0f,
                                  Math::Utils::Sin( l_Yaw + Math::pi32 / 2 ) );

   if ( pActionManager->DoAction( "MoveRight" ) )
     l_Direction -= Math::Vect3f( Math::Utils::Cos( l_Yaw + Math::pi32 / 2 ), 0.0f,
                                  Math::Utils::Sin( l_Yaw + Math::pi32 / 2 ) );

   if ( l_Direction != Math::Vect3f( 0.0f, 0.0f, 0.0f ) )
     l_Direction = l_Direction.GetNormalized();

   m_PhysicController->Move( l_Direction * 0.05f, deltaTime );

   if ( pActionManager->DoAction( "Jump" ) )
     m_PhysicController->Jump( 50 );
  //TODO CHARACTERCONTROLLER Descomentar para que la camara siga al character controller
  CCameraManager::GetSingletonPtr()->GetCurrentCamera()->SetPos( m_PhysicController->GetPosition() );*/


  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update()" );


  //////////////////////////////////////////////////////
  ////////////        DISPARO               ////////////
  //////////////////////////////////////////////////////

  if ( pActionManager->DoAction( "LeftMouseButtonPressed" ) )
  {
    CCamera* l_CurrentCamera =
      CCameraManager::GetSingletonPtr()->GetCurrentCamera();

    if ( l_CurrentCamera )
      m_Blaster->Update();
  }


  //////////////////////////////////////////////////////
  ////////////        UPDATE GRENADE        ////////////
  //////////////////////////////////////////////////////
  m_Grenade->Update();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////        INICIALIZACIÓN DE LA ESCENA PARA EL TEST DEL CHARACTER CONTROLLER      ///////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayerPhysicProcess::InitSceneCharacterController()
{
  //Scene Character Controller
  //Step1
  CPhysicUserData* l_PUD = new CPhysicUserData( "BoxCharacter1" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 1, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 0, 0, 0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );

  //Step2
  l_PUD = new CPhysicUserData( "BoxCharacter2" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 2, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 4, 0, 0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );

  //Step3
  l_PUD = new CPhysicUserData( "BoxCharacter3" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 3, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 8, 0, 0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );

  //Plano Inclinado TODO
  l_PUD = new CPhysicUserData( "Rampa" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 0.5f, 10, 4 ), Math::Vect3f( 0, 0, -5 ), Math::Vect3f( 3, 0, 0 ), Math::Vect3f( 0, 0, 1.3f ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
}

void CPlayerPhysicProcess::Init()
{
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();

  ////////////////////////////////////////////////////
  ////////////        CREATE GRENADE       ///////////
  ////////////////////////////////////////////////////
  m_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  m_Blaster = new CBlaster( 1.5f, 0.2f, 20.0f, "Glaster1" );

  /////////////////////////////////////////////////////////////////
  ////////////        CREATE CHARACTERCONTROLLER        ///////////
  /////////////////////////////////////////////////////////////////
  CPhysicUserData* userData = new CPhysicUserData( "CharacterController" );
  userData->SetPaint( true );
  userData->SetColor( colRED );
  m_vPUD.push_back( userData );
  
  Math::Vect3f l_Pos = Math::Vect3f( -143, 58, -87 );
  m_PhysicController = new CPhysicController( 2, 12, 0.2f, 0.5f, 0.5f, ECG_PLAYER,
      userData, l_Pos );
  l_PM->AddPhysicController( m_PhysicController );
  CCameraManager::GetSingletonPtr()->GetCurrentCamera()->SetPos(Math::Vect3f(l_Pos.x, l_Pos.y + (m_PhysicController->GetHeight()/2), l_Pos.z)  );


  //InitSceneCharacterController();

  //   // Create physic escene with ASE file
  //   CPhysicCookingMesh* l_PCM = new CPhysicCookingMesh();
  //   l_PCM->Init( l_PM->GetPhysicsSDK(), 0 );
  //   l_PCM->CreateMeshFromASE( "Data/a.ASE", "Escenario" );
  //
  //   CPhysicUserData* l_PUD2 = new CPhysicUserData( "Plane" );
  //   CPhysicActor* l_pActor = new CPhysicActor( l_PUD2 );
  //   //l_pActor3->AddMeshShape(CORE->GetPhysicsManager()->GetCookingMesh()->GetPhysicMesh("Malla_Fisicas"),Vect3f(-24.7306, 2.7749, -3.29779));
  //   l_pActor->AddMeshShape( l_PCM->GetPhysicMesh( "Escenario" ), Vect3f( 0, -5, 0 ) );

  CPhysicUserData* l_pPhysicUserDataASEMesh;
  CPhysicActor*  l_AseMeshActor;

  CPhysicCookingMesh* l_pMeshes = CCore::GetSingletonPtr()->GetPhysicsManager()->GetCookingMesh();
  CStaticMeshManager* l_StaticMeshManager = CCore::GetSingletonPtr()->GetStaticMeshManager();

  /* std::map<std::string, CStaticMesh*> l_MeshMap = l_StaticMeshManager->GetResourcesMap();

   std::map<std::string, CStaticMesh*>::iterator it = l_MeshMap.begin(), it_end = l_MeshMap.end();

   std::vector<std::vector<Math::Vect3f>> l_AllVB;
   std::vector<std::vector<uint32>> l_AllIB;

   for ( ; it != it_end; ++it )
   {
     CStaticMesh* l_StaticMesh = it->second;
     std::vector<Math::Vect3f> l_VB = l_StaticMesh->GetVB();
     std::vector<uint32> l_IB = l_StaticMesh->GetIB();

     l_AllVB.push_back( l_VB );
     l_AllIB.push_back( l_IB );

   }

   l_pMeshes->CreatePhysicMesh( "Escenario", l_AllVB, l_AllIB );

   l_pPhysicUserDataASEMesh = new CPhysicUserData( "Escenario" );
   l_pPhysicUserDataASEMesh->SetPaint( true );
   l_pPhysicUserDataASEMesh->SetColor( colRED );
   l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );
   m_vPUD.push_back( l_pPhysicUserDataASEMesh );

   VecMeshes l_CookMeshes = l_pMeshes->GetMeshes();

   for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
     l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

   CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor( l_AseMeshActor );
   m_vPA.push_back( l_AseMeshActor );*/


  if ( l_pMeshes->CreateMeshFromASE( "Data/a.ASE", "Escenario" ) )
  {
    l_pPhysicUserDataASEMesh = new CPhysicUserData( "Escenario" );
    m_vPUD.push_back( l_pPhysicUserDataASEMesh );
    l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

    VecMeshes l_CookMeshes = l_pMeshes->GetMeshes();

    for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
      l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

    //m_AseMeshActor->CreateBody ( 10.f );
    CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor( l_AseMeshActor );
    m_vPA.push_back( l_AseMeshActor );
  }

  m_pAStarScene = new CAStar();
  m_pAStarScene->Init();

  m_PointInicial = Math::Vect3f( 6, 0, -6 );
  m_PointFinal = Math::Vect3f( 6, 0, 6 );
  m_Path = m_pAStarScene->GetPath( m_PointInicial, m_PointFinal );

}

void CPlayerPhysicProcess::Render()
{
  CGraphicsManager* pGraphicsManager = GraphicsInstance;
  m_Grenade->Render();
  m_pAStarScene->Render();
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "render()" );
  // START: TO DELETE LATER IF IS NOT NECESSARY,
  unsigned int v = CGPUStatics::GetSingletonPtr()->GetVertexCount();
  unsigned int f = CGPUStatics::GetSingletonPtr()->GetFacesCount();
  unsigned int d = CGPUStatics::GetSingletonPtr()->GetDrawCount();
  CGPUStatics::GetSingletonPtr()->SetToZero();
  CCore::GetSingletonPtr()->GetFontManager()->DrawDefaultText( 300, 0,
      Math::CColor( 0.0f, 0.0f, 0.0f ), "Vertex: %u   Faces: %u   Draws:%u", v, f,
      d );
  // END: TO DELETE LATER IF IS NOT NECESSARY
}

void CPlayerPhysicProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}

CPhysicUserData* CPlayerPhysicProcess::GetNewPUD( const std::string& Name )
{
  return new CPhysicUserData( Name );
}
CPhysicActor* CPlayerPhysicProcess::GetNewPhysicActor( CPhysicUserData* PUD )
{
  return new CPhysicActor( PUD );
}

void CPlayerPhysicProcess::AddPudVector( CPhysicUserData* PUD )
{
  m_vPUD.push_back( PUD );
}
void CPlayerPhysicProcess::AddPhysicActorVector( CPhysicActor* PA )
{
  m_vPA.push_back( PA );
}
CPhysicUserData* CPlayerPhysicProcess::GetLastPUDInserted()
{
  return m_vPUD[m_vPUD.size() - 1];
}

CPhysicController*  CPlayerPhysicProcess::GetNewController( float _fRadius, float _fHeight,
    float _fSlope,
    float _fSkinwidth, float _fStepOffset,
    ECollisionGroup _uiCollisionGroups, CPhysicUserData* _pUserData, const Math::Vect3f& _vPos,
    float _fGravity )
{
  return new CPhysicController( _fRadius, _fHeight, _fSlope, _fSkinwidth, _fStepOffset,
                                _uiCollisionGroups, _pUserData, _vPos, _fGravity );
}

CCharacter* CPlayerPhysicProcess::GetNewCharacter( const std::string& Name )
{
  CCharacter* l_Character = new CCharacter( Name );
  m_vCharacter.push_back( l_Character );
  return l_Character;
}