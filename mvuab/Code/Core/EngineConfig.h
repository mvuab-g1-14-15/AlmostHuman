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

        GET_SET( bool , FullScreenMode );
        GET_SET( bool , EnableConsole );
        GET_SET( bool , ExclusiveModeInMouse );
        GET_SET( bool , DrawPointerMouse );
        GET_SET( bool , FitDesktop );
        GET_SET( uint32 , ScreenWidth );
        GET_SET( uint32 , ScreenHeight );
        GET_SET( uint32 , WindowXPos );
        GET_SET( uint32 , WindowYPos );
        GET_SET( uint32 , ResolutionWidth );
        GET_SET( uint32 , ResolutionHeight );
        GET_SET_REF( std::vector<std::string> , Languages );
        GET_SET_REF( std::string , CurrentLanguage );
        GET_SET_REF( std::string , ConfigPath );
        GET_SET_REF( std::string , GUIPath );
        GET_SET_REF( std::string , SoundPath );
        GET_SET_REF( std::string , FontsPath );
        GET_SET_REF( std::string , InputPath );
        GET_SET_REF( std::string , StaticMeshesPath   );
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
        bool                            m_ExclusiveModeInMouse;
        bool                            m_DrawPointerMouse;
        bool                            m_FitDesktop;
        uint32                          m_ResolutionWidth;
        uint32                          m_ResolutionHeight;
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
};

#endif // ENGINE_CONFIG_H