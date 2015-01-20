#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager
{
	private:
		lua_State	*m_LS;
		typedef std::vector<std::string> TVectorLuaFiles;
		TVectorLuaFiles m_LuaFiles;
		TVectorLuaFiles m_LuaInitFiles;
		std::string		m_LuaPath;

	public:
		CScriptManager();
		~CScriptManager();

		void Initialize();
		void Destroy();
		void Reload();

		void RunCode(const std::string &Code);
		void RunFile(const std::string &FileName);

		void Load(const std::string &XMLFile);

		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();
};

#endif
