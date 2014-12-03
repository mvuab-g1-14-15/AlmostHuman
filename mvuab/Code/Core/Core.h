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
class CDebugWindowManager;

class CCore : public CSingleton<CCore>
{
private:

	// Configuration
	float32						m_FPS;
	HWND						m_WindowId;						// 3D render window handle
	bool						m_FullScreenMode;
	bool						m_ExclusiveModeInMouse;
	bool						m_DrawPointerMouse;
	uint32						m_ScreenWidth;
	uint32						m_ScreenHeight;
	uint32						m_WindowXPos;
	uint32						m_WindowYPos;
	std::string					m_CurrentLanguage;
	std::string					m_ConfigPath;
	std::string					m_GUIPath;
	std::string					m_SoundPath;
	std::string					m_FontsPath;
	std::string					m_InputPath;
	std::vector<std::string>	m_v_languages;

	// Managers
	CGraphicsManager*		m_pGraphicsManager;
	CInputManager*			m_pInputManager;
	CLanguageManager*		m_pLanguageManager;
	CFontManager*			m_pFontManager;
	CActionManager*			m_pActionManager;
	CDebugWindowManager*	m_pDebugWindowManager;

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
	GET_SET(uint32, ScreenWidth);
	GET_SET(uint32, ScreenHeight);
	GET_SET(uint32, WindowXPos);
	GET_SET(uint32, WindowYPos);
	GET_SET(bool, FullScreenMode);
	GET_SET(bool, ExclusiveModeInMouse);
	GET_SET(bool, DrawPointerMouse);

	GET_SET(CInputManager*, pInputManager);
	GET_SET(CGraphicsManager*, pGraphicsManager);
	GET_SET(CFontManager*, pFontManager);
	GET_SET(CLanguageManager*, pLanguageManager);
	GET_SET(CActionManager*, pActionManager);
};

#endif // GRAPHICS_MANAGER_H
