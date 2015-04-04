#include "ScriptManager.h"
#include "FunctionsToRegister\BaseFunctions.h"
#include "FunctionsToRegister\CoreFunctions.h"
#include "FunctionsToRegister\GraphicsFunctions.h"
#include "FunctionsToRegister\InputFunctions.h"
#include "FunctionsToRegister\PhysXFunctions.h"
#include "FunctionsToRegister\VideogameFunctions.h"
#include "FunctionsToRegister\AIFunctions.h"
#include "FunctionsToRegister\EnumDefinitions.h"
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

  TVectorLuaFiles::iterator it = m_LuaFiles.begin(), it_end = m_LuaFiles.end();
  TVectorLuaFiles::iterator it_init = m_LuaInitFiles.begin(), it_init_end = m_LuaInitFiles.end();

  for ( ; it_init != it_init_end ; ++it_init )
    RunFile( m_LuaPath + "init/" + ( *it_init ) );

  for ( ; it != it_end; ++it )
    RunFile( m_LuaPath + ( *it ) );
}

void CScriptManager::RunCode( const std::string& Code )
{
  if ( luaL_dostring( m_LS, Code.c_str() ) )
  {
    const char* l_Str = lua_tostring( m_LS, -1 );
    assert( l_Str );
    LOG_ERROR_APPLICATION( "CScriptManager::RunCode\n\tCode: '%s'\n\tError: '%s'\n", Code.c_str(), l_Str );
  }
}

void CScriptManager::RunFile( const std::string& FileName )
{
  if ( luaL_dofile( m_LS, FileName.c_str() ) )
  {
    const char* l_Str = lua_tostring( m_LS, -1 );
    assert( l_Str );
    LOG_ERROR_APPLICATION( "CScriptManager::RunFile\n\tFile: '%s'\n\tError: '%s'\n", FileName.c_str(), l_Str );
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
  registerAI( m_LS );
  registerEnum( m_LS );
}
