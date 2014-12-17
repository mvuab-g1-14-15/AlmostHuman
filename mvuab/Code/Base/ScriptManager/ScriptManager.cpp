#include "ScriptManager.h"
#include <assert.h>


CScriptManager::CScriptManager()
{
}

void CScriptManager::Initialize()
{
	m_LS = luaL_newstate();
	luaL_openlibs(m_LS);

	assert(!"_ALERT");
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
	}
}

void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
	}
}
