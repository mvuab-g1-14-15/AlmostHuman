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
    .def( "SetSalir",  &CPhysicProcess::SetSalir )
    .def( "GetNewPhysicActor", &CPhysicProcess::GetNewPhysicActor )
    .def( "GetNewPUD", &CPhysicProcess::GetNewPUD )
    .def( "GetLastPUDInserted", &CPhysicProcess::GetLastPUDInserted )
    .def( "AddPhysicActorVector", &CPhysicProcess::AddPhysicActorVector )
    .def( "AddPudVector", &CPhysicProcess::AddPudVector )
  ];
}