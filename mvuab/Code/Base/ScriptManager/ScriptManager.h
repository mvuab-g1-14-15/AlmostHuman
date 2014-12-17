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
		lua_State *m_LS;

	public:
		CScriptManager();

		void Initialize();
		void Destroy();

		void RunCode(const std::string &Code) const;
		void RunFile(const std::string &FileName) const;
		void Load(const std::string &XMLFile);

		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();
};

#endif
