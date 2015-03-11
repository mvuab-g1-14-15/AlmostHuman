#include "VideogameFunctions.h"
#include "TestProcess\PhysicProcess.h"
#include "TestProcess\PlayerPhysicProcess.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Reports\PhysicTriggerReport.h"
#include "TestProcess\AStarProcess.h"
#include "Process.h"
#include "Characters/Character.h"
#include "Pathfinding\AStar.h"
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
  module( m_LS )
  [
    class_<CPlayerPhysicProcess, CProcess>( "CPlayerPhysicProcess" )
    .def( constructor<>() )
    .def( "GetNewPhysicActor", &CPlayerPhysicProcess::GetNewPhysicActor )
    .def( "GetNewPUD", &CPlayerPhysicProcess::GetNewPUD )
    .def( "GetLastPUDInserted", &CPlayerPhysicProcess::GetLastPUDInserted )
    .def( "AddPhysicActorVector", &CPlayerPhysicProcess::AddPhysicActorVector )
    .def( "AddPudVector", &CPlayerPhysicProcess::AddPudVector )
    .def( "GetNewController", &CPlayerPhysicProcess::GetNewController )
    .def( "GetNewCharacter", &CPlayerPhysicProcess::GetNewCharacter )
	.def( "GetPointInicial", &CPlayerPhysicProcess::GetPointInicial )
    .def( "SetPointInicial", &CPlayerPhysicProcess::SetPointInicial )
	.def( "GetPointFinal", &CPlayerPhysicProcess::GetPointFinal )
    .def( "SetPointFinal", &CPlayerPhysicProcess::SetPointFinal )
	.def( "GetAStarScene", &CPlayerPhysicProcess::GetAStarScene)
  ];
  module( m_LS )
  [
	class_<CAStarProcess, CProcess>( "CAStarProcess" )
    .def( constructor<>() )
	.def( "GetPointInicial", &CAStarProcess::GetPointInicial )
    .def( "SetPointInicial", &CAStarProcess::SetPointInicial )
	.def( "GetPointFinal", &CAStarProcess::GetPointFinal )
    .def( "SetPointFinal", &CAStarProcess::SetPointFinal )
	.def( "GetAStarScene", &CAStarProcess::GetAStarScene)
  ];
}