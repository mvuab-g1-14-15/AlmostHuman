#include "AStarProcess.h"

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


CAStarProcess::CAStarProcess() : CProcess()
{
}

CAStarProcess::~CAStarProcess()
{
  CHECKED_DELETE( m_AStarScene );
}

void CAStarProcess::Update()
{
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
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );

  m_AStarScene = new CAStar();
  m_AStarScene->Init();

  m_Path = m_AStarScene->GetPath( Math::Vect3f( 6, 0, -6 ), Math::Vect3f( 6, 0, 6 ) );
}

void CAStarProcess::Render()
{
  CGraphicsManager* pGraphicsManager = GraphicsInstance;

  m_AStarScene->Render();
  Math::Mat44f m;
  std::vector<Math::Vect3f>::iterator it = m_Path.begin(),
                                      it_end = m_Path.end();

  for ( ; it != it_end - 1; ++it )
  {
    Math::Vect3f l_ActualPoint = *it;
    Math::Vect3f l_NextPoint = *( it + 1 );

    pGraphicsManager->DrawLine( l_ActualPoint, l_NextPoint, Math::colMAGENTA );
  }

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "render()" );
}

void CAStarProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}
