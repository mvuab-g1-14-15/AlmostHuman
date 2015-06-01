#include "InputFunctions.h"

#include "InputManager.h"
#include "ActionManager.h"
#include "Utils\Manager.h"
#include "luabind_macros.h"

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
#include <luabind/out_value_policy.hpp>

using namespace luabind;

class CActionManagerLuaWrapper
{
public:
  float32 m_Value;
  CActionManagerLuaWrapper()
    : m_Value( 0.0f )
  {
  }
  bool DoAction( CActionManager& ActionManager, const std::string& ActionName )
  {
    return ActionManager.DoAction( ActionName, m_Value );
  }
};

void registerInputs( lua_State* m_LS )
{
  //INPUT MANAGER
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_DERIVED_CLASS( CInputManager, CManager )
  LUA_DECLARE_METHOD( CInputManager, IsDown )
  LUA_DECLARE_METHOD( CInputManager, IsDownUp )
  LUA_DECLARE_METHOD( CInputManager, IsUpDown )
  LUA_END_DECLARATION

  //ACTION MANAGER
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS( CActionManager )
  LUA_DECLARE_DEFAULT_CTOR
  LUA_DECLARE_METHOD( CActionManager, LoadXML )
  LUA_DECLARE_METHOD( CActionManager, SaveXML )
  LUA_DECLARE_METHOD_PROTO( CActionManager, DoAction, bool( CActionManager::* )( const std::string& ) )
  //luabind::pure_out_value(_3) -> Cuenta como primer parámetro el this de la clase, el segundo parámetro es el tercero realmente
  //http://stackoverflow.com/questions/14804154/luabind-pure-out-value-refuses-to-compile
  //.def("DoAction",(bool(CActionManager::*)(const std::string &, float32 &)) &CActionManager::DoAction, luabind::pure_out_value(_3))
  LUA_DECLARE_METHOD( CActionManager, SetAction )
  LUA_DECLARE_METHOD( CActionManager, Update )
  LUA_END_DECLARATION

  //ACTION MANAGER WRAPPER
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS( CActionManagerLuaWrapper )
  LUA_DECLARE_DEFAULT_CTOR
  .def_readwrite( "amount", &CActionManagerLuaWrapper::m_Value )
  LUA_DECLARE_METHOD( CActionManagerLuaWrapper, DoAction )
  LUA_END_DECLARATION
}