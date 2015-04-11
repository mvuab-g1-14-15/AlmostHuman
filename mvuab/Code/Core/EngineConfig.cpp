#include "EngineConfig.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

CEngineConfig::CEngineConfig()
  : m_FullScreenMode( false )
  , m_ExclusiveModeInMouse( false )
  , m_DrawPointerMouse( false )
  , m_FitDesktop( false )
  , m_ConfigPath( "" )
  , m_GUIPath( "" )
  , m_SoundPath( "" )
  , m_ManagersPath( "" )
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
    else if ( lTagName == "window_properties" )
    {
      m_ScreenResolution  = lTreeNode( i ).GetVect2iProperty( "resolution" , Math::Vect2i( 800, 600 ));
      m_ScreenSize        = lTreeNode( i ).GetVect2iProperty( "size", Math::Vect2i( 800, 600 ) );
      m_ScreenPosition    = lTreeNode( i ).GetVect2iProperty( "position", Math::Vect2i( 0, 0 ) );
      
      const std::string& lMode = lTreeNode( i ).GetPszProperty( "mode" );
      // Switch the modes of the screen
      m_FullScreenMode  = ( lMode == "full_screen" );
      m_FitDesktop      = ( lMode == "fit_desktop" );
      m_Windowed        = ( lMode == "windowed"    );

      if( lMode == "" )
      {
        LOG_ERROR_APPLICATION( "There is no mode for window, by default it would be 'fit_desktop'");
        m_FitDesktop = true;
      }

      // Get the refresh rate
      m_RefreshRate = lTreeNode( i ).GetIntProperty( "refresh_rate", 60 );
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
    else if ( lTagName == "managers" )
      m_ManagersPath = std::string( lTreeNode( i ).GetPszProperty( "path", "" ) );
  }
}

bool CEngineConfig::GetEnableConsole()
{
#ifdef _DEBUG
  return m_EnableConsole;
#else
  return false;
#endif
}

void CEngineConfig::SetEnableConsole( bool a_EnableConsole )
{
  m_EnableConsole = a_EnableConsole;
}

  const Math::Vect2i& CEngineConfig::GetScreenResolution() const
  {
    return m_ScreenResolution;
  }

  void CEngineConfig::SetScreenResolution( const Math::Vect2i & aVec )
  {
    m_ScreenResolution = aVec;
  }

  const Math::Vect2i& CEngineConfig::GetScreenPosition() const
  {
    return m_ScreenPosition;
  }

  void CEngineConfig::SetScreenPosition( const Math::Vect2i & aVec )
  {
    m_ScreenPosition = aVec;
  }

  const Math::Vect2i& CEngineConfig::GetScreenSize() const
  {
    return m_ScreenSize;
  }

  void CEngineConfig::SetScreenSize( const Math::Vect2i & aVec )
  {
    m_ScreenSize = aVec;
  }