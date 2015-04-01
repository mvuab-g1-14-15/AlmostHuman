#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H
#pragma once

#include "Utils/SingletonPattern.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"

#include <vector>
#include <string>

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
  std::string                     m_EnemiesPath;
  std::string                     m_ParticlesPath;
  std::string                     m_BillboardTexturePath;

  std::vector<std::string>        m_Languages;
public:
  CEngineConfig( const std::string& aCfg );
  virtual ~CEngineConfig(){ }

  GET_SET( bool , FullScreenMode );
  GET_SET( bool , ExclusiveModeInMouse );
  GET_SET( bool , DrawPointerMouse );
  GET_SET( uint32 , ScreenWidth );
  GET_SET( uint32 , ScreenHeight );
  GET_SET( uint32 , WindowXPos );
  GET_SET( uint32 , WindowYPos );
  GET_SET( std::vector<std::string> , Languages );
  GET_SET( std::string , CurrentLanguage );
  GET_SET( std::string , ConfigPath );
  GET_SET( std::string , GUIPath );
  GET_SET( std::string , SoundPath );
  GET_SET( std::string , FontsPath );
  GET_SET( std::string , InputPath );
  GET_SET( std::string , StaticMeshesPath   );
  GET_SET( std::string , RenderableObjectsPath );
  GET_SET( std::string , RenderableObjectTechniquePath );
  GET_SET( std::string , AnimatedModelsPath );
  GET_SET( std::string , LuaRunPath );
  GET_SET( std::string , EffectsPath );
  GET_SET( std::string , LightsPath );
  GET_SET( std::string , SceneRendererCommandPath );
  GET_SET( std::string , TriggersPath );
  GET_SET( std::string , EnemiesPath );
  GET_SET( std::string , ParticlesPath );
  GET_SET( std::string , BillboardTexturePath );


  /*bool FullScreenMode() const;
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
  void Languages(std::vector<std::string> val);*/
};

#endif // ENGINE_CONFIG_H