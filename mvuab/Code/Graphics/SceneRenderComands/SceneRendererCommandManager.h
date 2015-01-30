#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"
#include <string>

class CSceneRendererCommandManager
{
	private:
		CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;

		void CleanUp();
		std::string GetNextName();
    public:
        CSceneRendererCommandManager();
        ~ CSceneRendererCommandManager();

        bool Load(const std::string &FileName);
        bool Execute();
};
#endif