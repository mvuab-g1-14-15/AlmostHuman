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
#include "Utils\Manager.h"

//CORE

#include "EngineConfig.h"
#include "EngineManagers.h"

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

//GUI
#include "GUIManager.h"

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
  CActionManager* pActionManager = ActionManagerInstance;

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    SMeshMInstance->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
  {
    // ScriptMInstance->RunCode( "reload()" );
    ScriptMInstance->Reload();
    //ScriptMInstance->RunCode( "init()" );
  }
  if ( pActionManager->DoAction( "ReloadGUI" ) )
	  GUIInstance->Reload();

  /*  if ( pActionManager->DoAction( "ChangeRoom" ) )
      ScriptMInstance->RunCode( "cambiar_sala()" );*/

  if ( pActionManager->DoAction( "ChangeCamera" ) )
  {
    if ( "FreeCam" == CameraMInstance->GetCurrentCameraName() )
      CameraMInstance->SetCurrentCamera( "TestProcessCam" );
    else
      CameraMInstance->SetCurrentCamera( "FreeCam" );
  }

  if ( pActionManager->DoAction( "ReloadShaders" ) )
  {
    // NOTE this must be in this order
    EffectManagerInstance->Reload();
    LightMInstance->ReLoad();
    ROTMInstance->ReLoad();
    SMeshMInstance->Reload();
    ROLMInstance->Reload();
    LightMInstance->ReLoad();
    SRCMInstance->ReLoad();
  }

  if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    ActionManagerInstance->Reload();

  //ScriptMInstance->RunCode( "update()" );
  ScriptMInstance->RunCode( "update_gameplay()" );

  //////////////////////////////////////////////////////
  ////////////        DISPARO               ////////////
  //////////////////////////////////////////////////////

  if ( pActionManager->DoAction( "LeftMouseButtonPressed" ) )
  {
    CCamera* l_CurrentCamera =
      CameraMInstance->GetCurrentCamera();

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
  PhysXMInstance->AddActor( "BoxCharacter1", l_Type, Math::Vect3f( 2, 1, 2 ), colWHITE, true, Math::Vect3f( 0, 20, 5 ),
                            v3fZERO,
                            v3fZERO, 0, 0 );

  //Step2
  l_Type = "Box";
  PhysXMInstance->AddActor( "BoxCharacter2", l_Type, Math::Vect3f( 2, 2, 2 ), colWHITE, true, Math::Vect3f( 0, 21, 5 ),
                            Math::Vect3f( 4, 0, 0 ), v3fZERO, 0, 0 );

  //Step3
  l_Type = "Box";
  PhysXMInstance->AddActor( "BoxCharacter3", l_Type, Math::Vect3f( 2, 3, 2 ), colWHITE, true, Math::Vect3f( 0, 22, 5 ),
                            Math::Vect3f( 8, 0, 0 ), v3fZERO, 0, 0 );

  //Plano Inclinado TODO
  l_Type = "Box";
  PhysXMInstance->AddActor( "Rampa", l_Type, Math::Vect3f( 0.5f, 10, 4 ), colWHITE, true, Math::Vect3f( 0, 20, -5 ),
                            Math::Vect3f( 3, 0, 0 ), Math::Vect3f( 0, 0, 1.3f ), 0, 0 );
}

void CPlayerPhysicProcess::Init()
{
  //ScriptMInstance->RunCode( "init()" );
  ScriptMInstance->RunCode( "load_gameplay()" );
  CPhysicsManager* l_PM = PhysXMInstance;
  /*CWWSoundManager* l_SM = SoundMan;

  uint32 l_source1 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source1, Math::Vect3f( 2.0 ) );
  l_SM->SetSourceGain( l_source1, 100.0f );

  uint32 l_source2 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source2, Math::Vect3f( 5.0 ) );
  l_SM->SetSourceGain( l_source2, 100.0f );

  uint32 l_source3 =  l_SM->CreateSource();
  l_SM->SetSourcePosition( l_source3, Math::Vect3f( 5.0 ) );
  l_SM->SetSourceGain( l_source3, 100.0f );

  l_SM->SetListenerPosition( Math::Vect3f( 0.0 ) );
  l_SM->SetListenerOrientation( CameraMInstance->GetCurrentCamera()->GetDirection(),
                                CameraMInstance->GetCurrentCamera()->GetVecUp() );
  */
  //l_SM->PlayAction2D( "test" );
  //l_SM->PlaySource2D( l_source3 , "test", true );

  //CPhysicUserData* l_pPhysicUserDataASEMesh;
  //CPhysicActor*  l_AseMeshActor;

  //CPhysicCookingMesh* l_pMeshes = PhysXMInstance->GetCookingMesh();
  //CStaticMeshManager* l_StaticMeshManager = SMeshMInstance;

  //if ( l_pMeshes->CreateMeshFromASE( "Data/a.ASE", "Escenario" ) )
  //{
  //  l_pPhysicUserDataASEMesh = new CPhysicUserData( "Escenario" );
  //  l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
  //  m_vPUD.push_back( l_pPhysicUserDataASEMesh );
  //  l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

  //  VecMeshes l_CookMeshes = l_pMeshes->GetMeshes();

  //  for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
  //    l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

  //  //m_AseMeshActor->CreateBody ( 10.f );
  //  PhysXMInstance->AddPhysicActor( l_AseMeshActor );
  //  m_vPA.push_back( l_AseMeshActor );
  //}

  //Add Escenario
  if ( !PhysXMInstance->AddMesh( "Data/a.ASE", "Escenario" ) )
    LOG_ERROR_APPLICATION( "CPlayerPhysicProcess::Init No se pudo crear la malla Escenario!" );

  ////////////////////////////////////////////////////
  ////////////        CREATE GRENADE       ///////////
  ////////////////////////////////////////////////////
  m_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  m_Blaster = new CBlaster( 1.5f, 0.2f, 20.0f, "Glaster1" );

  m_AStar = new CAStar();
  m_AStar->Init();

  m_PointInicial = Math::Vect3f( 6, 0, -6 );
  m_PointFinal = Math::Vect3f( 6, 0, 6 );
  m_Path = m_AStar->GetPath( m_PointInicial, m_PointFinal );
  InitSceneCharacterController();
}

void CPlayerPhysicProcess::Render()
{
  m_Grenade->Render();
  m_AStar->Render();

  ScriptMInstance->RunCode( "render()" );
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

CPhysicController*  CPlayerPhysicProcess::GetNewController( float _fRadius, float _fHeight, float _fSlope,
    float _fSkinwidth, float _fStepOffset,
    CPhysicUserData* _pUserData, const Math::Vect3f& _vPos, float _fGravity )
{
  m_vController.push_back( new CPhysicController( _fRadius, _fHeight, _fSlope, _fSkinwidth, _fStepOffset, ECG_PLAYER,
                           _pUserData, _vPos, _fGravity ) );
  return m_vController[ m_vController.size() - 1 ];
}

void CPlayerPhysicProcess::DeleteController( CPhysicUserData* PUD )
{
  std::vector<CPhysicController*>::iterator it = std::find( m_vController.begin(), m_vController.end(),
      PUD->GetController() );

  if ( it != m_vController.end() )
  {
    CHECKED_DELETE( *it );
    std::vector<CPhysicUserData*>::iterator itPUD = std::find( m_vPUD.begin(), m_vPUD.end(), PUD );

    if ( itPUD != m_vPUD.end() )
      CHECKED_DELETE( *itPUD );
  }
}