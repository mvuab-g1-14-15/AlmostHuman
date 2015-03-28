#ifndef GRAPHICSREGISTER_H
#define GRAPHICSREGISTER_H

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "RenderableObject\RenderableObject.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "Math\Color.h"

#include "Object3D.h"

#include "Cameras/CameraManager.h"

#include "Math\Matrix44.h"

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

void registerGraphics(lua_State *m_LS);

#endif
