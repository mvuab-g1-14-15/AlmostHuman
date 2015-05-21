#ifndef SCRIPTREGISTER_H
#define SCRIPTREGISTER_H

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

void registerScript(lua_State *m_LS);

#endif //SCRIPTREGISTER_H
