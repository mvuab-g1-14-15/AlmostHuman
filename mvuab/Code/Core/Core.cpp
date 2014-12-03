#include "Core.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "Utils\DebugWindowManager.h"

CCore::CCore() :
	m_ConfigPath(""),
	m_GUIPath(""),
	m_SoundPath(""),
	m_InputPath(""),
	m_FontsPath(""),
	m_ScreenWidth(800),
	m_ScreenHeight(600),
	m_WindowXPos(0),
	m_WindowYPos(0),
	m_FullScreenMode(false),
	m_ExclusiveModeInMouse(false),
	m_DrawPointerMouse(false),
	m_pGraphicsManager( new CGraphicsManager() ),
	m_pInputManager( new CInputManager() ),
	m_pLanguageManager( new CLanguageManager() ),
	m_pFontManager( new CFontManager() ),
	m_pActionManager( new CActionManager() ),
	m_pDebugWindowManager( new CDebugWindowManager() )
{
}

CCore::~CCore()
{
	CHECKED_DELETE(m_pGraphicsManager);
	CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionManager);
	CHECKED_DELETE(m_pFontManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pDebugWindowManager);
}

void CCore::Init( const std::string & aConfigPath, HWND aWindowId )
{
	// Obtain the path of the configuration file and parse it
	m_ConfigPath = aConfigPath;
	m_WindowId = aWindowId;
	LoadXml();

	// Init the managers
	InitManagers();
}

void CCore::Update(float32 deltaTime)
{
	m_pGraphicsManager->Update(deltaTime);
	m_pInputManager->Update();
	m_pDebugWindowManager->Update(deltaTime);
	m_FPS = 1/ deltaTime;
}

void CCore::Render()
{
	m_pGraphicsManager->Render();
	m_pDebugWindowManager->Render();
	//uint32 idFont = m_pFontManager->GetTTF_Id("Annabel");
	//uint32 nextLine = m_pFontManager->DrawLiteral(0,0,"FPS");
	//m_pFontManager->DrawDefaultText(0,nextLine, colGREEN, "%d", (uint32) m_FPS);
}

void CCore::LoadXml()
{
	if(m_ConfigPath == "")
		return;

	CXMLTreeNode l_File;
	if (!l_File.LoadFile(m_ConfigPath.c_str()))
	{
		std::string err = "ERROR reading the file " + m_ConfigPath;

		MessageBox(NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(EXIT_FAILURE);
	}

	CXMLTreeNode  TreeNode =l_File["config"];
	if(TreeNode.Exists())
	{
		int count = TreeNode.GetNumChildren();
		for( int i = 0; i < count; ++i )
		{
			std::string TagName = TreeNode(i).GetName();
			if( TagName == "screen_resolution" )
			{
				m_ScreenWidth = TreeNode(i).GetIntProperty("width");
				m_ScreenHeight = TreeNode(i).GetIntProperty("height");
			}
			else if( TagName == "window_position" )
			{
				m_WindowXPos = TreeNode(i).GetIntProperty("x_pos", 0);
				m_WindowYPos = TreeNode(i).GetIntProperty("y_pos", 0);
			}
			else if( TagName == "render_mode" )
			{
				m_FullScreenMode = TreeNode(i).GetBoolProperty("fullscreen_mode", false);
			}          
			else if( TagName == "mouse" )
			{
				m_ExclusiveModeInMouse = TreeNode(i).GetBoolProperty("exclusive_mode_in_mouse", false);
				m_DrawPointerMouse = TreeNode(i).GetBoolProperty("draw_pointer_mouse", false);
			}
			else if( TagName == "GUI" )
			{
				m_GUIPath = std::string( TreeNode(i).GetPszProperty("init_gui_path", "") );
			}
			else if( TagName == "sound" )
			{
				m_SoundPath = std::string( TreeNode(i).GetPszProperty("init_sound_path", "") );
			}
			else if( TagName == "fonts" )
			{
				m_FontsPath = std::string( TreeNode(i).GetPszProperty("fonts_path", "") );
			}
			else if( TagName == "input" )
			{
				m_InputPath = std::string( TreeNode(i).GetPszProperty("path", "") );
			}
			else if( TagName == "languages" )
			{
				m_CurrentLanguage = std::string( TreeNode(i).GetPszProperty("current_language", "") );
				CXMLTreeNode  SubTreeNode =l_File["languages"];
				if(SubTreeNode.Exists())
				{
					int countLan = SubTreeNode.GetNumChildren();
					for( int lans = 0; lans < countLan; ++lans )
					{
						std::string TagName = SubTreeNode(lans).GetName();
						if( TagName == "language" )
							m_v_languages.push_back(std::string( SubTreeNode(lans).GetPszProperty("path", "") ));
					}
				}
				m_FontsPath = std::string( TreeNode(i).GetPszProperty("fonts_path", "") );
			}
		}
	}
}

void CCore::InitManagers()
{
	m_pGraphicsManager->Init( m_WindowId, m_FullScreenMode, m_ScreenWidth, m_ScreenHeight);
	m_pInputManager->Init( m_WindowId, Vect2i( m_ScreenWidth, m_ScreenHeight ), m_ExclusiveModeInMouse );
	m_pActionManager->Init( m_InputPath, m_pInputManager );
	m_pLanguageManager->SetXmlPaths( m_v_languages );
	m_pLanguageManager->LoadXMLs();
	m_pLanguageManager->SetCurrentLanguage(m_CurrentLanguage);
	m_pFontManager->Init(m_pGraphicsManager);
	m_pFontManager->LoadTTFs(m_FontsPath);
}