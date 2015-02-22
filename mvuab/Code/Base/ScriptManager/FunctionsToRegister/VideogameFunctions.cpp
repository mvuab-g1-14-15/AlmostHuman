#include "VideogameFunctions.h"
#include "TestProcess\PhysicProcess.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Reports\PhysicTriggerReport.h"
#include "Process.h"

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

using namespace luabind;

void registerVideogame( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CPhysicProcess, CProcess, CPhysicTriggerReport>( "CPhysicProcess" )
    .def( constructor<>() )
    /*.property( "m_Salir", &CPhysicProcess::SetSalir, &CPhysicProcess::GetSalir )
    .property( "m_PhysicActor", &CPhysicProcess::SetPhysicActor, &CPhysicProcess::GetPhysicActor )
    .property( "m_PUD", &CPhysicProcess::SetPUD, &CPhysicProcess::GetPUD )*/
    .def( "SetSalir",  &CPhysicProcess::SetSalir )
    .def( "GetSalir",  &CPhysicProcess::GetSalir )
    .def( "SetPhysicActor", &CPhysicProcess::SetPhysicActor )
    .def( "GetPhysicActor", &CPhysicProcess::GetPhysicActor )
    .def( "SetPUD", &CPhysicProcess::SetPUD )
    .def( "GetPUD", &CPhysicProcess::GetPUD )
  ];
}