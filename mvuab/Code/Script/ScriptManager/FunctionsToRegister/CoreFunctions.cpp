#include "CoreFunctions.h"

#include "Engine.h"
#include "Core.h"
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
#include "ScriptManager\ScriptManager.h"
#include "Cameras\CameraManager.h"
#include "Lights\LightManager.h"
#include "PhysicsManager.h"
#include "Process.h"
#include "TestProcess\PhysicProcess.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Characters\Enemies\EnemyManager.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

void registerCore( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CSingleton<CCore>>( "Singleton_Core" )
    .scope
    [
      def( "get_singleton", &CSingleton<CCore>::GetSingletonPtr )
    ]
  ];
  module( m_LS )
  [
    class_<CSingleton<CEngine>>( "Singleton_Engine" )
    .scope
    [
      def( "get_singleton", &CSingleton<CEngine>::GetSingletonPtr )
    ]
  ];
  module( m_LS )
  [
    class_<CProcess>( "CProcess" )
  ];
  module( m_LS )
  [
    class_<CEngine>( "CEngine" )
    .def( "GetProcess", &CEngine::GetProcess )
  ];
  module( m_LS )
  [
    class_<CCore>( "CCore" )
    //Methods
    .def( "Init", &CCore::Init )
    .def( "Update", &CCore::Update )
    .def( "Render", &CCore::Render )
    //Getter Managers
    .def( "GetActionManager", &CCore::GetActionManager )
    .def( "GetLightManager", &CCore::GetLightManager )
    .def( "GetAnimatedModelsManager", &CCore::GetAnimatedModelsManager )
    .def( "GetFontManager", &CCore::GetFontManager )
    .def( "GetGraphicsManager", &CCore::GetGraphicsManager )
    .def( "GetInputManager", &CCore::GetInputManager )
    .def( "GetLanguageManager", &CCore::GetLanguageManager )
    .def( "GetRenderableObjectsManager", &CCore::GetRenderableObjectsManager )
    .def( "GetScriptManager", &CCore::GetScriptManager )
    .def( "GetStaticMeshManager", &CCore::GetStaticMeshManager )
    .def( "GetTextureManager", &CCore::GetTextureManager )
    .def( "GetCameraManager", &CCore::GetCameraManager )
    .def( "GetTimer", &CCore::GetTimer )
    .def( "GetPhysicsManager", &CCore::GetPhysicsManager )
    .def( "GetEnemyManager", &CCore::GetEnemyManager )
    //Getter Variables
    .def( "GetAnimatedModelsPath", &CCore::GetAnimatedModelsPath )
    .def( "GetConfigPath", &CCore::GetConfigPath )
    .def( "GetDrawPointerMouse", &CCore::GetDrawPointerMouse )
    .def( "GetExclusiveModeInMouse", &CCore::GetExclusiveModeInMouse )
    .def( "GetFullScreenMode", &CCore::GetFullScreenMode )
    .def( "GetGUIPath", &CCore::GetGUIPath )
    .def( "GetScreenHeight", &CCore::GetScreenHeight )
    .def( "GetScreenWidth", &CCore::GetScreenWidth )
    .def( "GetSoundPath", &CCore::GetSoundPath )
    .def( "GetWindowXPos", &CCore::GetWindowXPos )
    .def( "GetWindowYPos", &CCore::GetWindowYPos )
    //Getter Singleton
    .def( "GetSingleton", &CCore::GetSingleton )
    .def( "GetSingletonPtr", &CCore::GetSingletonPtr )
    //Setter Managers
    .def( "SetActionManager", &CCore::SetActionManager )
    .def( "SetAnimatedModelsManager", &CCore::SetAnimatedModelsManager )
    .def( "SetFontManager", &CCore::SetFontManager )
    .def( "SetGraphicsManager", &CCore::SetGraphicsManager )
    .def( "SetInputManager", &CCore::SetInputManager )
    .def( "SetLanguageManager", &CCore::SetLanguageManager )
    .def( "SetRenderableObjectsManager", &CCore::SetRenderableObjectsManager )
    .def( "SetScriptManager", &CCore::SetScriptManager )
    .def( "SetStaticMeshManager", &CCore::SetStaticMeshManager )
    .def( "SetTextureManager", &CCore::SetTextureManager )
    .def( "SetPhysicsManager", &CCore::SetPhysicsManager )
    //Setter Variables
    .def( "SetAnimatedModelsPath", &CCore::SetAnimatedModelsPath )
    .def( "SetConfigPath", &CCore::SetConfigPath )
    .def( "SetDrawPointerMouse", &CCore::SetDrawPointerMouse )
    .def( "SetExclusiveModeInMouse", &CCore::SetExclusiveModeInMouse )
    .def( "SetFullScreenMode", &CCore::SetFullScreenMode )
    .def( "SetGUIPath", &CCore::SetGUIPath )
    .def( "SetScreenHeight", &CCore::SetScreenHeight )
    .def( "SetScreenWidth", &CCore::SetScreenWidth )
    .def( "SetSoundPath", &CCore::SetSoundPath )
    .def( "SetWindowXPos", &CCore::SetWindowXPos )
    .def( "SetWindowYPos", &CCore::SetWindowYPos )
    // Other
    .def( "trace", &CCore::Trace )
  ];
}