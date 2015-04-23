#include "VideogameFunctions.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Reports\PhysicTriggerReport.h"
#include "Process.h"
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
}