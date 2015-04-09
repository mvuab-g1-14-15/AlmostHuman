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
public:
  CEngineConfig();
  virtual ~CEngineConfig();
  void Load( const std::string& aCfg );

  bool GetEnableConsole();
  void SetEnableConsole( bool a_EnableConsole );

  const Math::Vect2i& GetScreenResolution() const;
  void SetScreenResolution( const Math::Vect2i & aVec );

  const Math::Vect2i& GetScreenPosition() const;
  void SetScreenPosition( const Math::Vect2i & aVec );

  const Math::Vect2i& GetScreenSize() const;
  void SetScreenSize( const Math::Vect2i & aVec );

  GET_SET( float, MaxFps);
  GET_SET( bool , FullScreenMode );
  GET_SET( bool , Windowed );
  GET_SET( bool , ExclusiveModeInMouse );
  GET_SET( HWND , WindowId );
  GET_SET( bool , DrawPointerMouse );
  GET_SET( bool , FitDesktop );
  GET_SET_REF( std::vector<std::string> , Languages );
  GET_SET_REF( std::string , CurrentLanguage );
  GET_SET_REF( std::string , ConfigPath );
  GET_SET_REF( std::string , GUIPath );
  GET_SET_REF( std::string , SoundPath );
  GET_SET_REF( std::string , FontsPath );
  GET_SET_REF( std::string , InputPath );
  GET_SET_REF( std::string , StaticMeshesPath );
  GET_SET_REF( std::string , RenderableObjectsPath );
  GET_SET_REF( std::string , RenderableObjectTechniquePath );
  GET_SET_REF( std::string , AnimatedModelsPath );
  GET_SET_REF( std::string , LuaRunPath );
  GET_SET_REF( std::string , EffectsPath );
  GET_SET_REF( std::string , LightsPath );
  GET_SET_REF( std::string , SceneRendererCommandPath );
  GET_SET_REF( std::string , TriggersPath );
  GET_SET_REF( std::string , EnemiesPath );
  GET_SET_REF( std::string , ParticlesPath );
  GET_SET_REF( std::string , BillboardTexturePath );

private:
  bool                            m_EnableConsole;
  bool                            m_FullScreenMode;
  bool                            m_FitDesktop;
  bool                            m_Windowed;
  bool                            m_ExclusiveModeInMouse;
  bool                            m_DrawPointerMouse;
  HWND                            m_WindowId;
  Math::Vect2i                    m_ScreenResolution;
  Math::Vect2i                    m_ScreenSize;
  Math::Vect2i                    m_ScreenPosition;
  float                           m_MaxFps;
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
};

#endif // ENGINE_CONFIG_H