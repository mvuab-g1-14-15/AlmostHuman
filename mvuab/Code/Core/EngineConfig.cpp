#include "EngineConfig.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

CEngineConfig::CEngineConfig( const std::string& aCfg )
{
  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( aCfg.c_str() ) )
  {
    std::string err = "ERROR reading the file " + m_ConfigPath;
    MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
    exit( EXIT_FAILURE );
  }

  CXMLTreeNode  TreeNode = l_File["config"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      std::string TagName = TreeNode( i ).GetName();

      if ( TagName == "screen_resolution" )
      {
        m_ScreenWidth = TreeNode( i ).GetIntProperty( "width" );
        m_ScreenHeight = TreeNode( i ).GetIntProperty( "height" );
      }
      else if ( TagName == "window_position" )
      {
        m_WindowXPos = TreeNode( i ).GetIntProperty( "x_pos", 0 );
        m_WindowYPos = TreeNode( i ).GetIntProperty( "y_pos", 0 );
      }
      else if ( TagName == "render_mode" )
        m_FullScreenMode = TreeNode( i ).GetBoolProperty( "fullscreen_mode", false );
      else if ( TagName == "mouse" )
      {
        m_ExclusiveModeInMouse = TreeNode(
                                   i ).GetBoolProperty( "exclusive_mode_in_mouse", false );
        m_DrawPointerMouse = TreeNode( i ).GetBoolProperty( "draw_pointer_mouse",
                             false );
      }
      else if ( TagName == "GUI" )
        m_GUIPath = std::string( TreeNode( i ).GetPszProperty( "init_gui_path", "" ) );
      else if ( TagName == "sound" )
        m_SoundPath = std::string( TreeNode( i ).GetPszProperty( "init_sound_path",
                                   "" ) );
      else if ( TagName == "fonts" )
        m_FontsPath = std::string( TreeNode( i ).GetPszProperty( "fonts_path", "" ) );
      else if ( TagName == "input" )
        m_InputPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "languages" )
      {
        m_CurrentLanguage = std::string( TreeNode(
                                           i ).GetPszProperty( "current_language", "" ) );
        CXMLTreeNode  SubTreeNode = l_File["languages"];

        if ( SubTreeNode.Exists() )
        {
          int countLan = SubTreeNode.GetNumChildren();

          for ( int lans = 0; lans < countLan; ++lans )
          {
            std::string TagName = SubTreeNode( lans ).GetName();

            if ( TagName == "language" )
              m_Languages.push_back( std::string( SubTreeNode(
                                                      lans ).GetPszProperty( "path", "" ) ) );
          }
        }

        m_FontsPath = std::string( TreeNode( i ).GetPszProperty( "fonts_path", "" ) );
      }
      else if ( TagName == "animated_models" )
        m_AnimatedModelsPath = std::string( TreeNode( i ).GetPszProperty( "path",
                                            "" ) );
      else if ( TagName == "static_meshes" )
        m_StaticMeshesPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "renderable_objects" )
        m_RenderableObjectsPath = std::string( TreeNode( i ).GetPszProperty( "path",
                                               "" ) );
      else if ( TagName == "renderable_object_technique" )
        m_RenderableObjectTechniquePath = std::string( TreeNode(
                                            i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "lua" )
        m_LuaRunPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "effects" )
        m_EffectsPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "lights" )
        m_LightsPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "scene_renderer_commands" )
        m_SceneRendererCommandPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "triggers" )
        m_TriggersPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "enemies" )
        m_EnemiesPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "particles" )
        m_ParticlesPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
      else if ( TagName == "billboardTexture" )
        m_BillboardTexturePath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
    }
  }
}
/*
bool CEngineConfig::CEngineConfig::FullScreenMode() const 
{ return m_FullScreenMode; }

void CEngineConfig::CEngineConfig::FullScreenMode(bool val)
{ m_FullScreenMode = val; }

bool CEngineConfig::CEngineConfig::ExclusiveModeInMouse() const
{ return m_ExclusiveModeInMouse; }

void CEngineConfig::CEngineConfig::ExclusiveModeInMouse(bool val)
{ m_ExclusiveModeInMouse = val; }

bool CEngineConfig::CEngineConfig::DrawPointerMouse() const
{ return m_DrawPointerMouse; }

void CEngineConfig::CEngineConfig::DrawPointerMouse(bool val)
{ m_DrawPointerMouse = val; }

uint32 CEngineConfig::ScreenWidth() const
{ return m_ScreenWidth; }

void CEngineConfig::ScreenWidth(uint32 val)
{ m_ScreenWidth = val; }

uint32 CEngineConfig::ScreenHeight() const
{ return m_ScreenHeight; }

void CEngineConfig::ScreenHeight(uint32 val)
{ m_ScreenHeight = val; }

uint32 CEngineConfig::WindowXPos() const
{ return m_WindowXPos; }

void CEngineConfig::WindowXPos(uint32 val)
{ m_WindowXPos = val; }

uint32 CEngineConfig::WindowYPos() const
{ return m_WindowYPos; }

void CEngineConfig::WindowYPos(uint32 val)
{ m_WindowYPos = val; }

std::string CurrentLanguage() const
{ return m_CurrentLanguage; }

void CEngineConfig::CurrentLanguage(std::string val)
{ m_CurrentLanguage = val; }

std::string GUIPath() const
{ return m_GUIPath; }

void CEngineConfig::GUIPath(std::string val)
{ m_GUIPath = val; }

std::string ConfigPath() const
{ return m_ConfigPath; }

void CEngineConfig::ConfigPath(std::string val)
{ m_ConfigPath = val; }

std::string SoundPath() const
{ return m_SoundPath; }

void CEngineConfig::SoundPath(std::string val)
{ m_SoundPath = val; }

std::string FontsPath() const
{ return m_FontsPath; }

void CEngineConfig::FontsPath(std::string val)
{ m_FontsPath = val; }

std::string InputPath() const
{ return m_InputPath; }

void CEngineConfig::InputPath(std::string val)
{ m_InputPath = val; }

std::string StaticMeshesPath() const
{ return m_StaticMeshesPath; }

void CEngineConfig::StaticMeshesPath(std::string val)
{ m_StaticMeshesPath = val; }

std::string RenderableObjectsPath() const
{ return m_RenderableObjectsPath; }

void CEngineConfig::RenderableObjectsPath(std::string val)
{ m_RenderableObjectsPath = val; }

std::string RenderableObjectTechniquePath() const
{ return m_RenderableObjectTechniquePath; }

void CEngineConfig::RenderableObjectTechniquePath(std::string val)
{ m_RenderableObjectTechniquePath = val; }

std::string AnimatedModelsPath() const
{ return m_AnimatedModelsPath; }

void CEngineConfig::AnimatedModelsPath(std::string val)
{ m_AnimatedModelsPath = val; }

std::string LuaRunPath() const
{ return m_LuaRunPath; }

void CEngineConfig::LuaRunPath(std::string val) 
{ m_LuaRunPath = val; }

std::string EffectsPath() const
{ return m_EffectsPath; }

void CEngineConfig::EffectsPath(std::string val)
{ m_EffectsPath = val; }

std::string LightsPath() const
{ return m_LightsPath; }

void CEngineConfig::LightsPath(std::string val)
{ m_LightsPath = val; }

std::string SceneRendererCommandPath() const
{ return m_SceneRendererCommandPath; }

void CEngineConfig::SceneRendererCommandPath(std::string val)
{ m_SceneRendererCommandPath = val; }

std::string TriggersPath() const
{ return m_TriggersPath; }

void CEngineConfig::TriggersPath(std::string val)
{ m_TriggersPath = val; }

std::vector<std::string> Languages() const
{ return m_Languages; }

void CEngineConfig::Languages(std::vector<std::string> val)
{ m_Languages = val; }
*/