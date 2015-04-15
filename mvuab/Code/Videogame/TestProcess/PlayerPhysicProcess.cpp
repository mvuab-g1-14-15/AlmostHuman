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
#include "ScriptManager.h"

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

//SOUND
#include "SoundManager.h"
#include "WWSoundManager.h"


#include <algorithm>
#include "RenderableVertex\VertexTypes.h"

CPlayerPhysicProcess::CPlayerPhysicProcess() : CProcess()
{
  unsigned short i = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_INDICES | VERTEX_TYPE_WEIGHT | VERTEX_TYPE_NORMAL |
                     VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
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

  for ( size_t i = 0; i < m_vController.size(); ++i )
    CHECKED_DELETE( m_vController[i] );

  m_vController.clear();
  //CHECKED_DELETE( m_PhysicController );
  CHECKED_DELETE( m_AStar );
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
  {
    // CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "reload()" );
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();
    //CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  }

  /* if ( pActionManager->DoAction( "ChangeRoom" ) )
     CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "cambiar_sala()" );*/

  if ( pActionManager->DoAction( "ChangeCamera" ) )
  {
    if ( "FreeCam" == CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCameraName() )
      CCore::GetSingletonPtr()->GetCameraManager()->SetCurrentCamera( "TestProcessCam" );
    else
      CCore::GetSingletonPtr()->GetCameraManager()->SetCurrentCamera( "FreeCam" );
  }

  if ( pActionManager->DoAction( "ReloadShaders" ) )
  {
    // NOTE this must be in this order
    CEffectManager::GetSingletonPtr()->Reload();
    CLightManager::GetSingletonPtr()->ReLoad();
    CRenderableObjectTechniqueManager::GetSingletonPtr()->ReLoad();
    CStaticMeshManager::GetSingletonPtr()->Reload();
    CRenderableObjectsLayersManager::GetSingletonPtr()->Reload();
    CLightManager::GetSingletonPtr()->ReLoad();
    CSceneRendererCommandManager::GetSingletonPtr()->ReLoad();
  }

  if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    CCore::GetSingletonPtr()->GetActionManager()->Reload();

  if ( pActionManager->DoAction( "Shoot" ) )
  {
    CWWSoundManager* l_SM = CCore::GetSingletonPtr()->GetSoundManager();
    l_SM->PauseEvent( "Shoot", "TestGameObject3d" );
  }

  CWWSoundManager* l_SM = CCore::GetSingletonPtr()->GetSoundManager();
  l_SM->SetListenerPosition( CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetPosition(),
                             CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetDirection(),
                             CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetVecUp() );
  //l_SM->SetGameObjectPosition( "TestGameObject3d", Vect3f( 0.0f, 0.0f, 0.0f ), Vect3f( 0.0f, 0.0f, 0.0f ) );
  //CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update()" );
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update_gameplay()" );


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
  std::string l_Type = "Box";
  CPhysicsManager::GetSingletonPtr()->AddActor( "BoxCharacter1", l_Type, Math::Vect3f( 2, 1, 2 ), colWHITE, true, Math::Vect3f( 0, 20, 5 ), v3fZERO,
      v3fZERO, 0, 0 );

  //Step2
  l_Type = "Box";
  CPhysicsManager::GetSingletonPtr()->AddActor( "BoxCharacter2", l_Type, Math::Vect3f( 2, 2, 2 ), colWHITE, true, Math::Vect3f( 0, 21, 5 ),
      Math::Vect3f( 4, 0, 0 ), v3fZERO, 0, 0 );

  //Step3
  l_Type = "Box";
  CPhysicsManager::GetSingletonPtr()->AddActor( "BoxCharacter3", l_Type, Math::Vect3f( 2, 3, 2 ), colWHITE, true, Math::Vect3f( 0, 22, 5 ),
      Math::Vect3f( 8, 0, 0 ), v3fZERO, 0, 0 );

  //Plano Inclinado TODO
  l_Type = "Box";
  CPhysicsManager::GetSingletonPtr()->AddActor( "Rampa", l_Type, Math::Vect3f( 0.5f, 10, 4 ), colWHITE, true, Math::Vect3f( 0, 20, -5 ),
      Math::Vect3f( 3, 0, 0 ), Math::Vect3f( 0, 0, 1.3f ), 0, 0 );

}

void CPlayerPhysicProcess::Init()
{
  //CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "load_gameplay()" );
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
  //CSoundManager* l_SM = CCore::GetSingletonPtr()->GetSoundManager();

  /*uint32 l_source1 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source1, Math::Vect3f( 2.0 ) );
  l_SM->SetSourceGain( l_source1, 100.0f );

  uint32 l_source2 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source2, Math::Vect3f( 5.0 ) );
  l_SM->SetSourceGain( l_source2, 100.0f );

  uint32 l_source3 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source3, Math::Vect3f( 5.0 ) );
  l_SM->SetSourceGain( l_source3, 100.0f );

  l_SM->SetListenerPosition( Math::Vect3f( 0.0 ) );
  l_SM->SetListenerOrientation( CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetDirection(),
                                CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetVecUp() );

  //l_SM->PlayAction2D( "test" );
  l_SM->PlaySource2D( l_source3 , "test", true );
  */

  ////////////////////////////////////////////////////
  ////////////        CREATE GRENADE       ///////////
  ////////////////////////////////////////////////////
  m_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  m_Blaster = new CBlaster( 1.5f, 0.2f, 20.0f, "Glaster1" );

  //Add Escenario
  if ( !CCore::GetSingletonPtr()->GetPhysicsManager()->AddMesh( "Data/a.ASE", "Escenario" ) )
    LOG_ERROR_APPLICATION( "CPlayerPhysicProcess::Init No se pudo crear la malla Escenario!" );

  m_AStar = new CAStar();
  m_AStar->Init();

  m_PointInicial = Math::Vect3f( 6, 0, -6 );
  m_PointFinal = Math::Vect3f( 6, 0, 6 );
  m_Path = m_AStar->GetPath( m_PointInicial, m_PointFinal );
  InitSceneCharacterController();

  CWWSoundManager* l_SM = CCore::GetSingletonPtr()->GetSoundManager();
  l_SM->PlayEvent( "SHOOT", "TestGameObject3d" );
}

void CPlayerPhysicProcess::Render()
{
  m_Grenade->Render();
  m_AStar->Render();

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "render()" );
  m_Blaster->Render();
}

void CPlayerPhysicProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}

CPhysicUserData* CPlayerPhysicProcess::GetNewPUD( const std::string& Name )
{
  m_vPUD.push_back( new CPhysicUserData( Name ) );
  return m_vPUD[m_vPUD.size() - 1];
}
CPhysicActor* CPlayerPhysicProcess::GetNewPhysicActor( CPhysicUserData* PUD )
{
  m_vPA.push_back( new CPhysicActor( PUD ) );
  return m_vPA[m_vPA.size() - 1];
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

CPhysicController*  CPlayerPhysicProcess::GetNewController( float _fRadius, float _fHeight, float _fSlope, float _fSkinwidth, float _fStepOffset,
    CPhysicUserData* _pUserData, const Math::Vect3f& _vPos, float _fGravity )
{
  m_vController.push_back( new CPhysicController( _fRadius, _fHeight, _fSlope, _fSkinwidth, _fStepOffset, ECG_PLAYER, _pUserData, _vPos, _fGravity ) );
  return m_vController[ m_vController.size() - 1 ];
}

void CPlayerPhysicProcess::DeleteController( CPhysicUserData* PUD )
{
  std::vector<CPhysicController*>::iterator it = std::find( m_vController.begin(), m_vController.end(), PUD->GetController() );

  if ( it != m_vController.end() )
  {
    CHECKED_DELETE( *it );
    std::vector<CPhysicUserData*>::iterator itPUD = std::find( m_vPUD.begin(), m_vPUD.end(), PUD );

    if ( itPUD != m_vPUD.end() )
      CHECKED_DELETE( *itPUD );
  }
}