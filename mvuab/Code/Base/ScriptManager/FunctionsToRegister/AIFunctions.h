#ifndef AIFUNCTIONS_H
#define AIFUNCTIONS_H

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>


using namespace luabind;

void registerAI(lua_State *m_LS);

#endif
