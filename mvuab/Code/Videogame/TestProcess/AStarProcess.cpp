#include "AStarProcess.h"

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

CAStarProcess::CAStarProcess() : CProcess()
{
}

CAStarProcess::~CAStarProcess()
{
  CHECKED_DELETE( m_pAStarScene );

  for ( size_t i = 0; i < m_vPA.size(); ++i )
    CHECKED_DELETE( m_vPA[i] );

  m_vPA.clear();

  for ( size_t i = 0; i < m_vPUD.size(); ++i )
    CHECKED_DELETE( m_vPUD[i] );

  m_vPUD.clear();

  CHECKED_DELETE( m_PhysicController );
}

void CAStarProcess::Update()
{
  /////////////////////////////////////////////////////////////
  ////////////      RELOADS ACTIONS           /////////////////
  /////////////////////////////////////////////////////////////
  CActionManager* pActionManager = ActionManagerInstance;

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    CCore::GetSingletonPtr()->GetStaticMeshManager()->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();

  if ( pActionManager->DoAction( "ReloadShaders" ) )
  {
    // NOTE this must be in this order
    EffectManagerInstance->Reload();
    CLightManager::GetSingletonPtr()->ReLoad();
    CRenderableObjectTechniqueManager::GetSingletonPtr()->ReLoad();
    CStaticMeshManager::GetSingletonPtr()->Reload();
    CRenderableObjectsLayersManager::GetSingletonPtr()->Reload();
    CSceneRendererCommandManager::GetSingletonPtr()->ReLoad();
  }

  if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    ActionManagerInstance->Reload();

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update()" );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////        INICIALIZACIÓN DE LA ESCENA PARA EL TEST DEL CHARACTER CONTROLLER      ///////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAStarProcess::InitSceneCharacterController()
{

}

void CAStarProcess::Init()
{
  //CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
  m_pAStarScene = new CAStar();
  m_pAStarScene->Init();

  CPhysicUserData* userData = new CPhysicUserData( "CharacterController" );
  userData->SetPaint( true );
  userData->SetColor( colWHITE );
  m_vPUD.push_back( userData );
  Math::Vect3f l_Pos = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetPosition();
  m_PhysicController = new CPhysicController( 0.5f, 2, 0.2f, 0.5f, 0.5f, ECG_PLAYER,
      userData, l_Pos );
  l_PM->AddPhysicController( m_PhysicController );

  CPhysicUserData* l_PUD = new CPhysicUserData( "Plane" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 1000, 0.0f, 1000 ), Math::Vect3f( 0, -0.5f,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );

  m_PointInicial = Math::Vect3f( 6, 0, -6 );
  m_PointFinal = Math::Vect3f( 6, 0, 6 );
  m_Path = m_pAStarScene->GetPath( m_PointInicial, m_PointFinal );
}

void CAStarProcess::Render()
{
  CGraphicsManager* pGraphicsManager = GraphicsInstance;

  m_pAStarScene->Render();
  Math::Mat44f m;
  std::vector<Math::Vect3f>::iterator it = m_Path.begin(),
                                      it_end = m_Path.end();


  for ( ; it != it_end - 1; ++it )
  {
    Math::Vect3f l_ActualPoint = *it;
    Math::Vect3f l_NextPoint = *( it + 1 );

    pGraphicsManager->DrawLine( l_ActualPoint, l_NextPoint, Math::colMAGENTA );
  }

  /*m.SetIdentity();
  m.Translate(m_PointInicial);
  pGraphicsManager->SetTransform(m);
  pGraphicsManager->DrawCube(0.2f, Math::colGREEN);
  m.SetIdentity();
  m.Translate(m_PointFinal);
  pGraphicsManager->SetTransform(m);
  pGraphicsManager->DrawCube(0.2f, Math::colRED);
  m.SetIdentity();
  pGraphicsManager->SetTransform(m);*/

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "render()" );
}

void CAStarProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}
