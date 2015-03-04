#include "ScriptManager\FunctionsToRegister\AIFunctions.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Characters/Character.h"


using namespace luabind;

void registerAI(lua_State *m_LS)
{
	module( m_LS )
  [
    class_<CCharacter>( "CCharacter" )
    .def( constructor<const std::string &>() )
	.def( "SetTargetPosition", &CCharacter::SetTargetPosition)
	.def( "Init", (void(CCharacter::*)(void))&CCharacter::Init )
    .def( "Update", &CCharacter::Update)
    .def( "Render", &CCharacter::Render )
  ];
}