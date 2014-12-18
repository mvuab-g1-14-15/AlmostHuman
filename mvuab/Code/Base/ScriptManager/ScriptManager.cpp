#include "ScriptManager.h"
#include "ScriptManager\FunctionsToRegister\BaseFunctions.h"
#include "ScriptManager\FunctionsToRegister\CoreFunctions.h"
#include "ScriptManager\FunctionsToRegister\GraphicsFunctions.h"

#include <assert.h>

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;


CScriptManager::CScriptManager()
{
}

CScriptManager::~CScriptManager()
{
	Destroy();
}

void CScriptManager::Initialize()
{
	m_LS = luaL_newstate();

	luaL_openlibs(m_LS);
	luabind::open(m_LS);

	RegisterLUAFunctions();
}

void CScriptManager::Destroy()
{
	lua_close(m_LS);
}

void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
		assert(l_Str);
	}
}

void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
		assert(l_Str);
	}
}

void CScriptManager::RegisterLUAFunctions()
{
	registerBase(m_LS);
	registerCore(m_LS);
	registerGraphics(m_LS);
}
