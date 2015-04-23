#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "Utils\Manager.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <vector>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager : public CManager
{
	private:
		lua_State	*m_LS;
		typedef std::vector<std::string> TVectorLuaFiles;
		TVectorLuaFiles m_LuaFiles;
		TVectorLuaFiles m_LuaInitFiles;

	public:
		CScriptManager();
		CScriptManager(CXMLTreeNode& atts);
		~CScriptManager();

		void Init();
		void Destroy();
		void Reload();

		void Update(){}
		void Render(){}

		void RunCode(const std::string &Code);
		void RunFile(const std::string &FileName);

		void Load();

		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();
};

#endif
