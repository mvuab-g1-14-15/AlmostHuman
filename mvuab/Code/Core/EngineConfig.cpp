#include "EngineConfig.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "XML\XMLDocument.h"

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
    , m_ScreenResolution( Math::Vect2i( 800, 600 ) )
    , m_ScreenSize( Math::Vect2i( 800, 600 ) )
    , m_ScreenPosition( Math::Vect2i( 0, 0 ) )
{
}

CEngineConfig::~CEngineConfig()
{
}

void CEngineConfig::Load( const std::string& aCfg )
{
    bool lOk( true );
    m_ConfigPath = aCfg;
    CXMLDocument lXMLDoc( m_ConfigPath );
    if( lXMLDoc.Load() )
    {
        CXMLTreeNode lRootNode = lXMLDoc.GetNode("config");
        for( CXMLTreeNode lChildNode = lRootNode.FirstChild(); lChildNode.More(); lChildNode = lChildNode.NextChild() )
        {
            const std::string & lTagName = lChildNode.GetName();
            if ( lTagName == "console_logger" )
            {
                lOk = lOk && lChildNode.GetAttribute<bool>("activate_console", m_EnableConsole );
            }
            else if ( lTagName == "window_properties" )
            {
                lOk = lOk && lChildNode.GetAttribute<Math::Vect2i>("resolution", m_ScreenResolution );
                lOk = lOk && lChildNode.GetAttribute<Math::Vect2i>("size", m_ScreenSize );
                lOk = lOk && lChildNode.GetAttribute<Math::Vect2i>("position", m_ScreenPosition );

                std::string lMode;
                lOk = lOk && lChildNode.GetAttribute<std::string>("mode", lMode );

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
                lOk = lOk && lChildNode.GetAttribute<bool>("exclusive_mode_in_mouse", m_ExclusiveModeInMouse );
                lOk = lOk && lChildNode.GetAttribute<bool>("draw_pointer_mouse", m_DrawPointerMouse );
            }
            else if ( lTagName == "GUI" )
            {
                lOk = lOk && lChildNode.GetAttribute<std::string>("init_gui_path", m_GUIPath );
            }
            else if ( lTagName == "sound" )
            {
                lOk = lOk && lChildNode.GetAttribute<std::string>("init_sound_path", m_SoundPath );
            }
            else if ( lTagName == "managers" )
            {
                lOk = lOk && lChildNode.GetAttribute<std::string>("path", m_ManagersPath );
            }
            else if ( lTagName == "dataPath" )
            {
                lOk = lOk && lChildNode.GetAttribute<std::string>("path", m_DataPath );
            }
        }
    }

    ASSERT(lOk, "Error loading the engine config");
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