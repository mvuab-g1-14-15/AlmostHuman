#include "SoundFunctions.h"
#include "luabind_macros.h"
#include "Utils/Defines.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

void registerSound( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Cameras" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // WWSOUND MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CWWSoundManager )
  LUA_DECLARE_METHOD( CWWSoundManager, SetSwitch )
  LUA_DECLARE_METHOD( CWWSoundManager, RegisterGameObject )
  LUA_DECLARE_METHOD( CWWSoundManager, UnregisterGameObject )
  LUA_DECLARE_METHOD( CWWSoundManager, SetState )
  LUA_DECLARE_METHOD( CWWSoundManager, SetListenerPosition )
  LUA_DECLARE_METHOD( CWWSoundManager, SetGameObjectPosition )

  LUA_DECLARE_METHOD( CWWSoundManager, PlayEvent )
  LUA_DECLARE_METHOD( CWWSoundManager, PlayParticlesSFX )
  LUA_DECLARE_METHOD( CWWSoundManager, ResumeEvent )
  LUA_DECLARE_METHOD( CWWSoundManager, PauseEvent )
  LUA_DECLARE_METHOD( CWWSoundManager, ResumeAllFromGameObject )
  LUA_DECLARE_METHOD( CWWSoundManager, PauseAllFromGameObject )

  LUA_DECLARE_METHOD( CWWSoundManager, GetGameObjectMapById )
  LUA_DECLARE_METHOD( CWWSoundManager, SetGameObjectMapById )

  LUA_END_DECLARATION
}