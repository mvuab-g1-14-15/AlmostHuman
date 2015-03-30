#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H
#pragma once

#include "Utils/SingletonPattern.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"

class CEngineConfig : public CSingleton<CEngineConfig>
{
private:
  bool                            m_FullScreenMode;
  bool                            m_ExclusiveModeInMouse;
  bool                            m_DrawPointerMouse;
  uint32                          m_ScreenWidth;
  uint32                          m_ScreenHeight;  
  uint32                          m_WindowXPos;
  uint32                          m_WindowYPos;
  std::string                     m_CurrentLanguage;
  std::string                     m_ConfigPath;
  std::string                     m_GUIPath;
  std::string                     m_SoundPath;
  std::string                     m_FontsPath;
  std::string                     m_InputPath;
  std::string                     m_StaticMeshesPath;  
  std::string                     m_RenderableObjectsPath;
  std::string                     m_RenderableObjectTechniquePath;
  std::string                     m_AnimatedModelsPath;  
  std::string                     m_LuaRunPath;
  std::string                     m_EffectsPath;  
  std::string                     m_LightsPath;
  std::string                     m_SceneRendererCommandPath;  
  std::string                     m_TriggersPath;
  std::vector<std::string>        m_Languages;
public:
  CEngineConfig( const CXMLTreeNode& aCfg );
  ~CEngineConfig();

  bool FullScreenMode() const;
  void FullScreenMode(bool val);
  bool ExclusiveModeInMouse() const;
  void ExclusiveModeInMouse(bool val);
  bool DrawPointerMouse() const;
  void DrawPointerMouse(bool val);
  uint32 ScreenWidth() const;
  void ScreenWidth(uint32 val);
  uint32 ScreenHeight() const;
  void ScreenHeight(uint32 val);
  uint32 WindowXPos() const;
  void WindowXPos(uint32 val);
  uint32 WindowYPos() const;
  void WindowYPos(uint32 val);
  std::string CurrentLanguage() const;
  void CurrentLanguage(std::string val);
  std::string GUIPath() const;
  void GUIPath(std::string val);
  std::string ConfigPath() const;
  void ConfigPath(std::string val);
  std::string SoundPath() const;
  void SoundPath(std::string val);
  std::string FontsPath() const;
  void FontsPath(std::string val);
  std::string InputPath() const;
  void InputPath(std::string val);
  std::string StaticMeshesPath() const;
  void StaticMeshesPath(std::string val);
  std::string RenderableObjectsPath() const;
  void RenderableObjectsPath(std::string val);
  std::string RenderableObjectTechniquePath() const ;
  void RenderableObjectTechniquePath(std::string val);
  std::string AnimatedModelsPath() const;
  void AnimatedModelsPath(std::string val);
  std::string LuaRunPath() const;
  void LuaRunPath(std::string val);
  std::string EffectsPath() const;
  void EffectsPath(std::string val);
  std::string LightsPath() const;
  void LightsPath(std::string val);
  std::string SceneRendererCommandPath() const;
  void SceneRendererCommandPath(std::string val);
  std::string TriggersPath() const;
  void TriggersPath(std::string val);
  std::vector<std::string> Languages() const;
  void Languages(std::vector<std::string> val);
};

#endif // ENGINE_CONFIG_H