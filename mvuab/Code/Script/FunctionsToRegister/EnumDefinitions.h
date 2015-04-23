#ifndef ENUM_DEFINITIONS_H
#define ENUM_DEFINITIONS_H

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

void registerEnum( lua_State* m_LS );

#endif //ENUM_DEFINITIONS_H
