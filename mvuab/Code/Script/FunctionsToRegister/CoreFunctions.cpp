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
#include "EngineManagers.h"
#include "Particles\ParticleManager.h"

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

CEnemyManager* GetEnemyManager()
{
    return EnemyMInstance;
}

CRenderableObjectsLayersManager* GetRenderableObjectsLayersManager()
{
    return ROLMInstance;
}

CGizmosManager* GetGizmosManager()
{
    return GizmosMInstance;
}

CParticleManager* GetParticleManager()
{
    return ParticleMInstance;
}
void registerProcess( lua_State* aLuaState)
{
    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_CLASS( CProcess )
    LUA_END_DECLARATION
}

void registerEngine( lua_State* aLuaState)
{
    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_CLASS( CSingleton<CEngine> )
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_DERIVED_CLASS( CEngine, CSingleton<CEngine> )
    LUA_DECLARE_METHOD( CEngine, GetProcess )
    LUA_DECLARE_METHOD( CEngine, GetTimer )
    LUA_DECLARE_METHOD( CEngine, Trace )
    LUA_BEGIN_SCOPE
    LUA_DECLARE_METHOD_SCOPE(CEngine, GetSingletonPtr )
    LUA_END_SCOPE
    LUA_END_DECLARATION
}

void registerManagers( lua_State* aLuaState )
{
    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetActionManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetCameraManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetPhysicsManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetEnemyManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetRenderableObjectsLayersManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetGizmosManager)
    LUA_END_DECLARATION

    LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_METHOD_WITHOUT_CLASS( GetParticleManager)
    LUA_END_DECLARATION
}

void registerCore( lua_State* aLuaState )
{
    registerProcess( aLuaState );
    registerEngine( aLuaState );
    registerManagers( aLuaState );
}