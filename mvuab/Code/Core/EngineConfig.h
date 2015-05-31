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
        enum EOutputMode { eConsole = 0, eMVSOutputWindow };
    public:
        CEngineConfig();
        virtual ~CEngineConfig();
        void Load( const std::string& aCfg );

        EOutputMode GetLoggerOutputMode();
        EOutputMode GetTraceOutputMode();

        bool GetEnableConsole();
        void SetEnableConsole( bool a_EnableConsole );

        const Math::Vect2i& GetScreenResolution() const;
        void SetScreenResolution( const Math::Vect2i& aVec );

        const Math::Vect2i& GetScreenPosition() const;
        void SetScreenPosition( const Math::Vect2i& aVec );

        const Math::Vect2i& GetScreenSize() const;
        void SetScreenSize( const Math::Vect2i& aVec );

        GET_SET( bool , FullScreenMode );
        GET_SET( bool , Windowed );
        GET_SET( bool , ExclusiveModeInMouse );
        GET_SET( HWND , WindowId );
        GET_SET( bool , DrawPointerMouse );
        GET_SET( bool , FitDesktop );
        GET_SET_REF( std::string , ConfigPath );
        GET_SET_REF( std::string , GUIPath );
        GET_SET_REF( std::string , SoundPath );
        GET_SET_REF( std::string , ManagersPath );
        GET_SET_REF( std::string , DataPath );

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
        std::string                     m_ConfigPath;
        std::string                     m_GUIPath;
        std::string                     m_SoundPath;
        std::string                     m_ManagersPath;
        std::string                     m_DataPath;
        EOutputMode                     mLoggerOutpuMode;
        EOutputMode                     mTraceOutpuMode;
};

#endif // ENGINE_CONFIG_H