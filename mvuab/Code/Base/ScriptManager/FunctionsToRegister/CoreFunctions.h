#ifndef COREFUNCTIONS_H
#define COREFUNCTIONS_H

#include "Engine.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "Utils\DebugWindowManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "ScriptManager\ScriptManager.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

void registerCore(lua_State *m_LS);

#endif
