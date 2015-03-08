#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"
#include <string>
#include "Utils/SingletonPattern.h"

class CSceneRendererCommandManager : public CSingleton<CSceneRendererCommandManager>
{
public:
  CSceneRendererCommandManager();
  ~ CSceneRendererCommandManager();

  bool Load( const std::string& FileName );
  bool ReLoad();
  bool Execute();

private:
  CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;
  std::string m_FileName;

  void CleanUp();
  std::string GetNextName();
};
#endif