#include "ScriptFunctions.h"
#include "luabind_macros.h"

#include "ScriptManager.h"

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

void registerScript(lua_State *m_LS)
{
	LUA_BEGIN_DECLARATION( m_LS )
		LUA_DECLARE_DERIVED_CLASS( CScriptManager, CManager )
		LUA_DECLARE_METHOD( CScriptManager, RunCode )
		LUA_DECLARE_METHOD( CScriptManager, RunFile )
  LUA_END_DECLARATION
}