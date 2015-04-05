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
#include "RenderableObject\RenderableObjectsLayersManager.h"
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
        .def( "GetRenderableObjectsLayersManager", &CCore::GetRenderableObjectsLayersManager )
        .def( "GetTextureManager", &CCore::GetTextureManager )
        .def( "GetCameraManager", &CCore::GetCameraManager )
        .def( "GetTimer", &CCore::GetTimer )
        .def( "GetPhysicsManager", &CCore::GetPhysicsManager )
        .def( "GetEnemyManager", &CCore::GetEnemyManager )
        .def( "GetGizmosManager", &CCore::GetGizmosManager )
        //Getter Singleton
        .def( "GetSingleton", &CCore::GetSingleton )
        .def( "GetSingletonPtr", &CCore::GetSingletonPtr )
        // Other
        .def( "trace", &CCore::Trace )
    ];
}