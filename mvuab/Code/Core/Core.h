#ifndef CORE_H
#define CORE_H
#pragma once

// Includes
#include <string>
#include <vector>
#include <Windows.h>

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"

// Forward declarations
class CGraphicsManager;
class CInputManager;
class CLanguageManager;
class CFontManager;
class CActionManager;
class CTextureManager;
class CDebugWindowManager;
class CStaticMeshManager;
class CRenderableObjectsManager;
class CAnimatedModelsManager;

class CCore : public CSingleton<CCore>
{
private:

    // Configuration
    HWND                        m_WindowId;                        // 3D render window handle
    bool                        m_FullScreenMode;
    bool                        m_ExclusiveModeInMouse;
    bool                        m_DrawPointerMouse;
    uint32                        m_ScreenWidth;
    uint32                        m_ScreenHeight;
    uint32                        m_WindowXPos;
    uint32                        m_WindowYPos;
    std::string                    m_CurrentLanguage;
    std::string                    m_ConfigPath;
    std::string                    m_GUIPath;
    std::string                    m_SoundPath;
    std::string                    m_FontsPath;
    std::string                    m_InputPath;
    std::string                    m_StaticMeshesPath;
    std::string                    m_RenderableObjectsPath;
    std::string                    m_AnimatedModelsPath;
    std::vector<std::string>    m_v_languages;

    // Managers
    CGraphicsManager*               m_pGraphicsManager;
    CInputManager*                  m_pInputManager;
    CLanguageManager*               m_pLanguageManager;
    CFontManager*                   m_pFontManager;
    CActionManager*                 m_pActionManager;
    CDebugWindowManager*            m_pDebugWindowManager;
    CStaticMeshManager*             m_pStaticMeshManager;
    CRenderableObjectsManager*      m_pRenderableObjectsManager;
    CAnimatedModelsManager*         m_pAnimatedModelsManager;
    CTextureManager*                m_pTextureManager;

    void LoadXml();
    void InitManagers();

public:
    CCore();
    ~CCore();

    void Init( const std::string & aConfigPath, HWND aWindowId );
    void Update(float32 deltaTime);
    void Render();

    // Getters and setters
    GET_SET_REF(std::string, ConfigPath);
    GET_SET_REF(std::string, GUIPath);
    GET_SET_REF(std::string, SoundPath);
    GET_SET_REF(std::string, AnimatedModelsPath);
    GET_SET(uint32, ScreenWidth);
    GET_SET(uint32, ScreenHeight);
    GET_SET(uint32, WindowXPos);
    GET_SET(uint32, WindowYPos);
    GET_SET(bool, FullScreenMode);
    GET_SET(bool, ExclusiveModeInMouse);
    GET_SET(bool, DrawPointerMouse);

    GET_SET_PTR(CInputManager, InputManager);
    GET_SET_PTR(CGraphicsManager, GraphicsManager);
    GET_SET_PTR(CFontManager, FontManager);
    GET_SET_PTR(CLanguageManager, LanguageManager);
    GET_SET_PTR(CActionManager, ActionManager);
    GET_SET_PTR(CStaticMeshManager, StaticMeshManager);
    GET_SET_PTR(CRenderableObjectsManager, RenderableObjectsManager);
    GET_SET_PTR(CAnimatedModelsManager, AnimatedModelsManager);
    GET_SET_PTR(CTextureManager, TextureManager);
};

#endif // GRAPHICS_MANAGER_H
