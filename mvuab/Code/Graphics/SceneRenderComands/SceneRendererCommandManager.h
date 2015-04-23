#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"
#include <string>
#include "Utils/Manager.h"

class CSceneRendererCommandManager : public CManager
{
public:
  CSceneRendererCommandManager();
  CSceneRendererCommandManager(CXMLTreeNode& atts);
  ~ CSceneRendererCommandManager();

  void Init();
  void ReLoad();
  bool Execute();
  void Update(){}
  void Render(){}

private:
  CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;

  void CleanUp();
  std::string GetNextName();
};
#endif