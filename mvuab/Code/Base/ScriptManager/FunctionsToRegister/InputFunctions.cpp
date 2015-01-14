#include "InputFunctions.h"

#include "InputManager.h"

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

void registerInputs(lua_State *m_LS)
{
	module(m_LS)
	[
		class_<CInputManager>("CInputManager")
			.def(constructor<>())

			.def("IsDown", &CInputManager::IsDown)
			.def("IsDownUp", &CInputManager::IsDownUp)
			.def("IsUpDown", &CInputManager::IsUpDown)
	];
}