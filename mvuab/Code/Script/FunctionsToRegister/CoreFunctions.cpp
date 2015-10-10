#include "CoreFunctions.h"

#include "Engine.h"

#include "Timer\Timer.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\Scene.h"
#include "ScriptManager.h"
#include "Cameras\CameraManager.h"
#include "Lights\LightManager.h"
#include "PhysicsManager.h"
#include "Process.h"
#include "TestProcess\PhysicProcess.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Gizmos\GizmosManager.h"
#include "EngineManagers.h"
#include "WWSoundManager.h"
#include "GUIManager.h"
#include "Timer/CountDownTimerManager.h"
#include "Cinematics\CinematicManager.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "Billboard\BillboardManager.h"
#include "Triggers\TriggerManager.h"

#include "luabind_macros.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

CActionManager* GetActionManager()
{
  return ActionManagerInstance;
}

CCameraManager* GetCameraManager()
{
  return CameraMInstance;
}

CPhysicsManager* GetPhysicsManager()
{
  return PhysXMInstance;
}

CScene* GetScene()
{
  return SceneInstance;
}


CTextureManager* GetTextureManager()
{
  return TextureMInstance;
}

CTriggerManager* GetTriggerManager()
{
  return TriggersMInstance;
}

//CEnemyManager* GetEnemyManager()
//{
//  return EnemyMInstance;
//}


CGizmosManager* GetGizmosManager()
{
  return GizmosMInstance;
}

CWWSoundManager* GetSoundManager()
{
  return SoundMan;
}

CGUIManager* GetGUIManager()
{
  return GUIInstance;
}

CCountDownTimerManager* GetCountDownTimerManager()
{
  return CountDownTimerInstance;
}

CScriptManager* GetScriptManager()
{
  return ScriptMInstance;
}

CCinematicManager* GetCinematicManager()
{
  return CinematicMInstance;
}

CSceneRendererCommandManager* GetSceneRendererCommandsManager()
{
  return SRCMInstance;
}

CBillboardManager* GetBillboardManager()
{
  return BillboardMan;
}

CLightManager* GetLightManager()
{
  return LightMInstance;
}

void registerProcess( lua_State* aLuaState )
{
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CProcess )
  LUA_END_DECLARATION
}

void registerEngine( lua_State* aLuaState )
{
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CSingleton<CEngine> )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CEngine, CSingleton<CEngine> )
  LUA_DECLARE_METHOD( CEngine, GetProcess )
  LUA_DECLARE_METHOD( CEngine, GetTimer )
  LUA_DECLARE_METHOD( CEngine, GetPlay )
  LUA_DECLARE_METHOD( CEngine, SetPlay )
  LUA_DECLARE_METHOD( CEngine, QuitGame )
  LUA_DECLARE_METHOD( CEngine, Trace )
  LUA_BEGIN_SCOPE
  LUA_DECLARE_METHOD_SCOPE( CEngine, GetSingletonPtr )
  LUA_END_SCOPE
  LUA_END_DECLARATION
}

void registerManagers( lua_State* aLuaState )
{
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CSingleton<CEngineManagers> )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CEngineManagers )
  LUA_DECLARE_METHOD( CEngineManagers, GetManagersPath )
  LUA_DECLARE_METHOD( CEngineManagers, SetManagersPath )
  LUA_DECLARE_METHOD( CEngineManagers, GetbGamePause )
  LUA_DECLARE_METHOD( CEngineManagers, SetbGamePause )
  LUA_BEGIN_SCOPE
  LUA_DECLARE_METHOD_SCOPE( CEngineManagers, GetSingletonPtr )
  LUA_END_SCOPE
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetActionManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetCameraManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetPhysicsManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetTriggerManager )
  LUA_END_DECLARATION

  /*LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetEnemyManager )
  LUA_END_DECLARATION*/

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetTextureManager )
  LUA_END_DECLARATION

  /*LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetRenderableObjectsLayersManager )
  LUA_END_DECLARATION*/

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetGizmosManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetSoundManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetGUIManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetCountDownTimerManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetScriptManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetCinematicManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetBillboardManager )
  LUA_END_DECLARATION


  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetLightManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetScene )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetSceneRendererCommandsManager )
  LUA_END_DECLARATION
}

void registerCore( lua_State* aLuaState )
{
  registerProcess( aLuaState );
  registerEngine( aLuaState );
  registerManagers( aLuaState );
}