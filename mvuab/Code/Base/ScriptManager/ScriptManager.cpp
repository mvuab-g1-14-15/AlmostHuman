#include "ScriptManager.h"
#include "ScriptManager\FunctionsToRegister\BaseFunctions.h"
#include "ScriptManager\FunctionsToRegister\CoreFunctions.h"
#include "ScriptManager\FunctionsToRegister\GraphicsFunctions.h"
#include "ScriptManager\FunctionsToRegister\InputFunctions.h"
#include "ScriptManager\FunctionsToRegister\PhysXFunctions.h"
#include "ScriptManager\FunctionsToRegister\VideogameFunctions.h"
#include <string>
#include <iostream>

#include "Utils/BaseUtils.h"

#include <assert.h>

// Lua includes
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;


CScriptManager::CScriptManager() : m_LuaPath( "" )
{
}

CScriptManager::~CScriptManager()
{
  Destroy();
}

void CScriptManager::Initialize()
{
  m_LS = luaL_newstate();
  luaL_openlibs( m_LS );
  luabind::open( m_LS );
  RegisterLUAFunctions();
}

void CScriptManager::Destroy()
{
  lua_close( m_LS );
}

void CScriptManager::Reload()
{
  m_LuaFiles.clear();
  m_LuaInitFiles.clear();
  Destroy();
  Initialize();
  Load( m_LuaPath );
}

void CScriptManager::Load( const std::string& Path )
{
  m_LuaPath = Path;
  baseUtils::GetFilesFromPath( m_LuaPath, "lua", m_LuaFiles );
  baseUtils::GetFilesFromPath( m_LuaPath + "init/", "lua", m_LuaInitFiles );
  TVectorLuaFiles::iterator it = m_LuaInitFiles.begin(), it_end = m_LuaInitFiles.end();

  for ( ; it != it_end; ++it )
    RunFile( m_LuaPath + "init/" + ( *it ) );
}

void CScriptManager::RunCode( const std::string& Code )
{
  if ( luaL_dostring( m_LS, Code.c_str() ) )
  {
    const char* l_Str = lua_tostring( m_LS, -1 );
    assert( l_Str );
    std::cout << l_Str << std::endl << std::endl;
  }
}

void CScriptManager::RunFile( const std::string& FileName )
{
  if ( luaL_dofile( m_LS, FileName.c_str() ) )
  {
    const char* l_Str = lua_tostring( m_LS, -1 );
    assert( l_Str );
    std::cout << l_Str << std::endl << std::endl;
  }
}

void CScriptManager::RegisterLUAFunctions()
{
  registerBase( m_LS );
  registerCore( m_LS );
  registerGraphics( m_LS );
  registerInputs( m_LS );
  registerPhysX( m_LS );
  registerVideogame( m_LS );
}
