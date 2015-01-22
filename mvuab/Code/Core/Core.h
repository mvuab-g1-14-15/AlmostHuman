#ifndef CORE_H
#define CORE_H
#pragma once

// Includes
#include <string>
#include <vector>
#include <Windows.h>

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Math\Vector3.h"

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
class CScriptManager;
class CCameraManager;
class CEffectManager;
class CLightManager;
class CTimer;
class CRenderableObjectTechniqueManager;

class CCore : public CSingleton<CCore>
{
private:

    // Configuration
    HWND                            m_WindowId;
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
    std::vector<std::string>        m_v_languages;

    // Managers
    CGraphicsManager*						m_pGraphicsManager;
    CInputManager*							m_pInputManager;
    CLanguageManager*						m_pLanguageManager;
    CFontManager*							m_pFontManager;
    CActionManager*							m_pActionManager;
    CDebugWindowManager*					m_pDebugWindowManager;
    CStaticMeshManager*						m_pStaticMeshManager;
    CRenderableObjectsManager*				m_pRenderableObjectsManager;
    CAnimatedModelsManager*					m_pAnimatedModelsManager;
    CTextureManager*						m_pTextureManager;
    CScriptManager*							m_pScriptManager;
    CCameraManager*							m_pCameraManager;
    CEffectManager*							m_pEffectManager;
	CLightManager*							m_pLightManager;
	CTimer*									m_pTimer;
	CRenderableObjectTechniqueManager*		m_pRenderableObjectTechniqueManager;

    void LoadXml();
    void InitManagers();

public:
    CCore();
    ~CCore();

    void Init( const std::string & aConfigPath, HWND aWindowId );
    void Update();
    void Render();


	// Methods that trace Lua
	void TraceString(const std::string & TraceStr);
	void TraceInteger(int TraceInteger);
	void TraceFloat(float TraceFloat);
	void TraceVector( const Math::Vect3f & vector );

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
    GET_SET_PTR(CScriptManager, ScriptManager);
    GET_SET_PTR(CCameraManager, CameraManager);
    GET_SET_PTR(CEffectManager, EffectManager);
	GET_SET_PTR(CLightManager, LightManager);
	GET_SET_PTR(CTimer, Timer);
	GET_SET_PTR(CRenderableObjectTechniqueManager, RenderableObjectTechniqueManager);
};

#endif // GRAPHICS_MANAGER_H