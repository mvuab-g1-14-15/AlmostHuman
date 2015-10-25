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
    , m_DataPath( "" )
    , mLoggerOutpuMode( eMVSOutputWindow )
    , mTraceOutpuMode( eConsole )
    , m_EnableConsole( false )
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
        const CXMLTreeNode& lNode = lTreeNode(i);
        const std::string & lTagName = lNode.GetName();

        if( lTagName == "logger" )
        {
            const std::string& lLoggerMode = lNode.GetAttribute<std::string>("mode", "console");
            mLoggerOutpuMode = ( lLoggerMode == "console") ? eConsole : eMVSOutputWindow;
            m_EnableConsole = m_EnableConsole || ( lLoggerMode == "console");
        }
        else if( lTagName == "trace" )
        {
            const std::string& lTraceMode = lNode.GetAttribute<std::string>("mode", "console");
            mTraceOutpuMode = ( lTraceMode == "console") ? eConsole : eMVSOutputWindow;
            m_EnableConsole = m_EnableConsole || ( lTraceMode == "console");
        }
        else if ( lTagName == "window_properties" )
        {
            m_ScreenResolution        = lNode.GetAttribute<Math::Vect2i>( "resolution" , Math::Vect2i( 800, 600 ) );
            m_ScreenSize              = lNode.GetAttribute<Math::Vect2i>( "size", Math::Vect2i( 800, 600 ) );
            m_ScreenPosition          = lNode.GetAttribute<Math::Vect2i>( "position", Math::Vect2i( 0, 0 ) );
            const std::string& lMode  = lNode.GetAttribute<std::string>( "mode", "fit_desktop");
            // Switch the modes of the screen
            m_FullScreenMode  = ( lMode == "full_screen" );
            m_FitDesktop      = ( lMode == "fit_desktop" );
            m_Windowed        = ( lMode == "windowed" );

            if ( lMode == "" )
            {
                LOG_ERROR_APPLICATION( "There is no mode for window, by default it would be 'fit_desktop'" );
                m_FitDesktop = true;
            }
        }
        else if ( lTagName == "mouse" )
        {
            m_ExclusiveModeInMouse = lNode.GetAttribute<bool>( "exclusive_mode_in_mouse", false );
            m_DrawPointerMouse = lNode.GetAttribute<bool>( "draw_pointer_mouse", false );
        }
        else if ( lTagName == "GUI" )
        {
            m_GUIPath = lNode.GetAttribute<std::string>( "init_gui_path", "" );
        }
        else if ( lTagName == "sound" )
        {
            m_SoundPath = lNode.GetAttribute<std::string>( "init_sound_path", "" );
        }
        else if ( lTagName == "managers" )
        {
            m_ManagersPath = lNode.GetAttribute<std::string>( "path", "" );
        }
        else if ( lTagName == "dataPath" )
        {
            m_DataPath = lNode.GetAttribute<std::string>( "path", "" );
        }
    }
}

CEngineConfig::EOutputMode CEngineConfig::GetLoggerOutputMode()
{
    return mLoggerOutpuMode;
}

CEngineConfig::EOutputMode CEngineConfig::GetTraceOutputMode()
{
    return mTraceOutpuMode;
}

bool CEngineConfig::GetEnableConsole()
{
    return m_EnableConsole;
}

void CEngineConfig::SetEnableConsole( bool a_EnableConsole )
{
    m_EnableConsole = a_EnableConsole;
}

const Math::Vect2i& CEngineConfig::GetScreenResolution() const
{
    return m_ScreenResolution;
}

void CEngineConfig::SetScreenResolution( const Math::Vect2i& aVec )
{
    m_ScreenResolution = aVec;
}

const Math::Vect2i& CEngineConfig::GetScreenPosition() const
{
    return m_ScreenPosition;
}

void CEngineConfig::SetScreenPosition( const Math::Vect2i& aVec )
{
    m_ScreenPosition = aVec;
}

const Math::Vect2i& CEngineConfig::GetScreenSize() const
{
    return m_ScreenSize;
}

void CEngineConfig::SetScreenSize( const Math::Vect2i& aVec )
{
    m_ScreenSize = aVec;
}