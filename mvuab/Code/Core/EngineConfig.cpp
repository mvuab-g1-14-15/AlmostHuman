#include "EngineConfig.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

CEngineConfig::CEngineConfig()
  : m_CurrentLanguage( "" )
  , m_ConfigPath( "" )
  , m_GUIPath( "" )
  , m_SoundPath( "" )
  , m_FontsPath( "" )
  , m_InputPath( "" )
  , m_StaticMeshesPath( "" )
  , m_RenderableObjectsPath( "" )
  , m_RenderableObjectTechniquePath( "" )
  , m_AnimatedModelsPath( "" )
  , m_LuaRunPath( "" )
  , m_EffectsPath( "" )
  , m_LightsPath( "" )
  , m_SceneRendererCommandPath( "" )
  , m_TriggersPath( "" )
  , m_ScreenWidth( 800 )
  , m_ScreenHeight( 600 )
  , m_WindowXPos( 0 )
  , m_WindowYPos( 0 )
  , m_FullScreenMode( false )
  , m_ExclusiveModeInMouse( false )
  , m_DrawPointerMouse( false )
  , m_ResolutionWidth( 0 )
  , m_ResolutionHeight( 0 )
  , m_FitDesktop( false )
{
}

CEngineConfig::~CEngineConfig()
{
}

void CEngineConfig::Load( const std::string& aCfg )
{

  m_ConfigPath = aCfg;
  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( aCfg.c_str() ) )
  {
    std::string err = "Error reading the configuration file of the engine" + m_ConfigPath;
    FATAL_ERROR( err.c_str() );
  }

  CXMLTreeNode  lTreeNode = l_File["config"];

  if ( !lTreeNode.Exists() )
  {
    std::string err = "Error the file" + m_ConfigPath + "does not contain the tag <config>";
    FATAL_ERROR( err.c_str() );
  }

  int lCount = lTreeNode.GetNumChildren();

  for ( int i = 0; i < lCount; ++i )
  {
    const std::string& lTagName = lTreeNode( i ).GetName();

    if ( lTagName == "console_logger" )
      m_EnableConsole = lTreeNode( i ).GetBoolProperty( "activate_console", false );

    if ( lTagName == "screen_resolution" )
    {
      m_ResolutionWidth = lTreeNode( i ).GetIntProperty( "width" );
      m_ResolutionHeight = lTreeNode( i ).GetIntProperty( "height" );
    }
    else if ( lTagName == "window_position" )
    {
      m_WindowXPos = lTreeNode( i ).GetIntProperty( "x", 0 );
      m_WindowYPos = lTreeNode( i ).GetIntProperty( "y", 0 );
    }
    else if ( lTagName == "window_size" )
    {
      m_ScreenWidth       = lTreeNode( i ).GetIntProperty( "width" );
      m_ScreenHeight      = lTreeNode( i ).GetIntProperty( "height" );
      m_FullScreenMode    = lTreeNode( i ).GetBoolProperty( "fullscreen_mode", false );
      m_FitDesktop        = lTreeNode( i ).GetBoolProperty( "fit_desktop", false );
    }
    else if ( lTagName == "mouse" )
    {
      m_ExclusiveModeInMouse = lTreeNode(
                                 i ).GetBoolProperty( "exclusive_mode_in_mouse", false );
      m_DrawPointerMouse = lTreeNode( i ).GetBoolProperty( "draw_pointer_mouse",
                           false );
    }
    else if ( lTagName == "GUI" )
      m_GUIPath = std::string( lTreeNode( i ).GetPszProperty( "init_gui_path", "" ) );
    else if ( lTagName == "sound" )
      m_SoundPath = std::string( lTreeNode( i ).GetPszProperty( "init_sound_path",
                                 "" ) );
    else if ( lTagName == "fonts" )
      m_FontsPath = std::string( lTreeNode( i ).GetPszProperty( "fonts_path", "" ) );
    else if ( lTagName == "input" )
      m_InputPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "languages" )
    {
      m_CurrentLanguage = std::string( lTreeNode(
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

      m_FontsPath = std::string( lTreeNode( i ).GetPszProperty( "fonts_path", "" ) );
    }
    else if ( lTagName == "animated_models" )
      m_AnimatedModelsPath = std::string( lTreeNode( i ).GetPszProperty( "path",
                                          "" ) );
    else if ( lTagName == "static_meshes" )
      m_StaticMeshesPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "renderable_objects" )
      m_RenderableObjectsPath = std::string( lTreeNode( i ).GetPszProperty( "path",
                                             "" ) );
    else if ( lTagName == "renderable_object_technique" )
      m_RenderableObjectTechniquePath = std::string( lTreeNode(
                                          i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "lua" )
      m_LuaRunPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "effects" )
      m_EffectsPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "lights" )
      m_LightsPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "scene_renderer_commands" )
      m_SceneRendererCommandPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "triggers" )
      m_TriggersPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "enemies" )
      m_EnemiesPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "particles" )
      m_ParticlesPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
    else if ( lTagName == "billboardTexture" )
      m_BillboardTexturePath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
  }
}

bool CEngineConfig::GetEnableConsole()
{
#ifdef _DEBUG
  return m_EnableConsole;
#else
  return false;
#endif
};

void CEngineConfig::SetEnableConsole( bool a_EnableConsole )
{
  m_EnableConsole = a_EnableConsole;
};