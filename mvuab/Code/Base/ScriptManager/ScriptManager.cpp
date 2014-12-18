#include "ScriptManager.h"
#include "ScriptManager\FunctionsToRegister\BaseFunctions.h"
#include "ScriptManager\FunctionsToRegister\CoreFunctions.h"
#include "ScriptManager\FunctionsToRegister\GraphicsFunctions.h"

#include <assert.h>

// Lua includes
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>

#include <iostream>

using namespace luabind;

namespace fs = boost::filesystem;

CScriptManager::CScriptManager() : m_LuaPath("")
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

void CScriptManager::Reload()
{
	m_LuaFiles.clear();

	Destroy();

	Load(m_LuaPath);
	Initialize();
}

void CScriptManager::Load(const std::string &Path)
{
	m_LuaPath = Path;

	fs::path full_path( fs::initial_path<fs::path>() );

    full_path = fs::system_complete( fs::path( m_LuaPath ) );

  unsigned long file_count = 0;
  unsigned long err_count = 0;

  if ( !fs::exists( full_path ) )
  {
    //std::cout << "\nNot found: " << full_path.file_string() << std::endl;
    //return 1;
  }

  if ( fs::is_directory( full_path ) )
  {
    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( full_path ); dir_itr != end_iter; ++dir_itr )
    {
      try
      {
        if ( fs::is_directory( dir_itr->status() ) )
        {
          //std::cout << dir_itr->path().filename() << " [directory]\n";
        }
        else if ( fs::is_regular( dir_itr->status() ) )
        {
          ++file_count;
		  m_LuaFiles.push_back(dir_itr->string());
        }
      }
      catch ( const std::exception & ex )
      {
        ++err_count;
        //std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
      }
    }
  }
  else // must be a file
  {
    //std::cout << "\nFound: " << full_path.file_string() << "\n";    
  }
}

void CScriptManager::RunCode(const std::string &Code)
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
		assert(l_Str);
	}
}

void CScriptManager::RunFile(const std::string &FileName)
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
