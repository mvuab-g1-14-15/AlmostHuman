#include "PreProcess.h"

//BASE
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\MapManager.h"

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
#include "Billboard\Billboard.h"

//INPUTS
#include "InputManager.h"
#include "ActionManager.h"

//PHYSX
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "CookingMesh\PhysicCookingMesh.h"

//SOUND
#include "SoundManager.h"

//GUI
#include "GUIManager.h"


#include <algorithm>
#include "RenderableVertex\VertexTypes.h"

CPreProcess::CPreProcess() : CProcess()
{
  unsigned short i = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_INDICES | VERTEX_TYPE_WEIGHT | VERTEX_TYPE_NORMAL |
                     VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
}

CPreProcess::~CPreProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();
}

void CPreProcess::Update()
{
  /////////////////////////////////////////////////////////////
  ////////////      RELOADS ACTIONS           /////////////////
  /////////////////////////////////////////////////////////////
  CActionManager* pActionManager = ActionManagerInstance;

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    SMeshMInstance->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
    ScriptMInstance->Reload();

  if ( pActionManager->DoAction( "ReloadGUI" ) )
  {
    GUIInstance->Reload();
    ScriptMInstance->RunCode( "ReloadGUI()" );
  }

  if ( pActionManager->DoAction( "ReloadManagers" ) )
  {
    EngineManagerInstance->Reload();
    EngineManagerInstance->Update();
    pActionManager = ActionManagerInstance;
  }

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
}


void CPreProcess::Init()
{

}

void CPreProcess::Render()
{

}

void CPreProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}