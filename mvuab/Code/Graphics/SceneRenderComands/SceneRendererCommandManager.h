#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"
#include <string>
#include "Utils/SingletonPattern.h"

class CSceneRendererCommandManager : public CSingleton<CSceneRendererCommandManager>
{
private:
  CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;

  void CleanUp();
  std::string GetNextName();
public:
  CSceneRendererCommandManager();
  ~ CSceneRendererCommandManager();

  bool Load( const std::string& FileName );
  bool ReLoad();
  bool Execute();

private:
  std::string m_FileName;

};
#endif