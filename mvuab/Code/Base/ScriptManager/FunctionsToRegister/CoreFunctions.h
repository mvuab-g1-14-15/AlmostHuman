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

void registerCore(lua_State *m_LS)
{
	module(m_LS)
	[
		class_<CEngine>("CEngine")
			.def(constructor<>())
			//Methods
			.def("Init", &CEngine::Init)
			.def("Update", &CEngine::Update)
			.def("Render", &CEngine::Render)

			//Getter
			.def("GetpCore", &CEngine::GetpCore)

			//Getter Singleton
			.def("GetSingleton", &CEngine::GetSingleton)
			.def("GetSingletonPtr", &CEngine::GetSingletonPtr)
			//Setter
			.def("SetpCore", &CEngine::SetpCore)
			
	];
	module(m_LS)
	[
		class_<CCore>("CCore")
			.def(constructor<>())
			//Methods
			.def("Init", &CCore::Init)
			.def("Update", &CCore::Update)
			.def("Render", &CCore::Render)

			//Getter Managers
			.def("GetActionManager", &CCore::GetActionManager)	
			.def("GetAnimatedModelsManager", &CCore::GetAnimatedModelsManager)
			.def("GetFontManager", &CCore::GetFontManager)
			.def("GetGraphicsManager", &CCore::GetGraphicsManager)
			.def("GetInputManager", &CCore::GetInputManager)
			.def("GetLanguageManager", &CCore::GetLanguageManager)
			.def("GetRenderableObjectsManager", &CCore::GetRenderableObjectsManager)
			.def("GetScriptManager", &CCore::GetScriptManager)
			.def("GetStaticMeshManager", &CCore::GetStaticMeshManager)
			.def("GetTextureManager", &CCore::GetTextureManager)
			//Getter Variables
			.def("GetAnimatedModelsPath", &CCore::GetAnimatedModelsPath)
			.def("GetConfigPath", &CCore::GetConfigPath)
			.def("GetDrawPointerMouse", &CCore::GetDrawPointerMouse)
			.def("GetExclusiveModeInMouse", &CCore::GetExclusiveModeInMouse)
			.def("GetFullScreenMode", &CCore::GetFullScreenMode)
			.def("GetGUIPath", &CCore::GetGUIPath)
			.def("GetScreenHeight", &CCore::GetScreenHeight)
			.def("GetScreenWidth", &CCore::GetScreenWidth)
			.def("GetSoundPath", &CCore::GetSoundPath)
			.def("GetWindowXPos", &CCore::GetWindowXPos)
			.def("GetWindowYPos", &CCore::GetWindowYPos)
			//Getter Singleton
			.def("GetSingleton", &CCore::GetSingleton)
			.def("GetSingletonPtr", &CCore::GetSingletonPtr)
			//Setter Managers
			.def("SetActionManager", &CCore::SetActionManager)
			.def("SetAnimatedModelsManager", &CCore::SetAnimatedModelsManager)
			.def("SetFontManager", &CCore::SetFontManager)
			.def("SetGraphicsManager", &CCore::SetGraphicsManager)
			.def("SetInputManager", &CCore::SetInputManager)
			.def("SetLanguageManager", &CCore::SetLanguageManager)
			.def("SetRenderableObjectsManager", &CCore::SetRenderableObjectsManager)
			.def("SetScriptManager", &CCore::SetScriptManager)
			.def("SetStaticMeshManager", &CCore::SetStaticMeshManager)
			.def("SetTextureManager", &CCore::SetTextureManager)
			//Setter Variables
			.def("SetAnimatedModelsPath", &CCore::SetAnimatedModelsPath)
			.def("SetConfigPath", &CCore::SetConfigPath)
			.def("SetDrawPointerMouse", &CCore::SetDrawPointerMouse)
			.def("SetExclusiveModeInMouse", &CCore::SetExclusiveModeInMouse)
			.def("SetFullScreenMode", &CCore::SetFullScreenMode)
			.def("SetGUIPath", &CCore::SetGUIPath)
			.def("SetScreenHeight", &CCore::SetScreenHeight)
			.def("SetScreenWidth", &CCore::SetScreenWidth)
			.def("SetSoundPath", &CCore::SetSoundPath)
			.def("SetWindowXPos", &CCore::SetWindowXPos)
			.def("SetWindowYPos", &CCore::SetWindowYPos)
	];
	
}

#endif
