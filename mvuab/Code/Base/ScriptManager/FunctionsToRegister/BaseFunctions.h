#ifndef BASEFUNCTIONS_H
#define BASEFUNCTIONS_H

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Math\Matrix44.h"
#include "Math\Matrix34.h"
#include "Math\Vector3.h"
#include "Utils\Visible.h"
#include "Utils\Name.h"

using namespace luabind;

void registerBase(lua_State *m_LS);

#endif
