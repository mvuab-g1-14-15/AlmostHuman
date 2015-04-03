#include "InputFunctions.h"

#include "InputManager.h"
#include "ActionManager.h"

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
  module( m_LS )
  [
    class_<CInputManager>( "CInputManager" )
    .def( constructor<>() )

    .def( "IsDown", &CInputManager::IsDown )
    .def( "IsDownUp", &CInputManager::IsDownUp )
    .def( "IsUpDown", &CInputManager::IsUpDown )
  ];

  module( m_LS )
  [
    class_<CActionManager>( "CActionManager" )
    .def( constructor<>() )


    .def( "LoadXML", &CActionManager::LoadXML )
    .def( "SaveXML", &CActionManager::SaveXML )
    .def( "DoAction", ( bool( CActionManager::* )( const std::string& ) ) &CActionManager::DoAction )
    //luabind::pure_out_value(_3) -> Cuenta como primer parámetro el this de la clase, el segundo parámetro es el tercero realmente
    //http://stackoverflow.com/questions/14804154/luabind-pure-out-value-refuses-to-compile
    //.def("DoAction",(bool(CActionManager::*)(const std::string &, float32 &)) &CActionManager::DoAction, luabind::pure_out_value(_3))
    .def( "SetAction", &CActionManager::SetAction )
    .def( "Update", &CActionManager::Update )
  ];

  module( m_LS )
  [
    class_<CActionManagerLuaWrapper>( "CActionManagerLuaWrapper" )
    .def( constructor<>() )
    .def_readwrite( "amount", &CActionManagerLuaWrapper::m_Value )
    .def( "DoAction", &CActionManagerLuaWrapper::DoAction )
  ];
}