#include "Core.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "Utils\DebugWindowManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"

CCore::CCore() :
    m_ConfigPath(""),
    m_GUIPath(""),
    m_SoundPath(""),
    m_InputPath(""),
    m_FontsPath(""),
    m_AnimatedModelsPath(""),
    m_StaticMeshesPath(""),
    m_RenderableObjectsPath(""),
    m_ScreenWidth(800),
    m_ScreenHeight(600),
    m_WindowXPos(0),
    m_WindowYPos(0),
    m_FullScreenMode(false),
    m_ExclusiveModeInMouse(false),
    m_DrawPointerMouse(false),
    m_pTextureManager( new CTextureManager() ),
    m_pGraphicsManager( new CGraphicsManager() ),
    m_pInputManager( new CInputManager() ),
    m_pLanguageManager( new CLanguageManager() ),
    m_pFontManager( new CFontManager() ),
    m_pActionManager( new CActionManager() ),
    m_pDebugWindowManager( new CDebugWindowManager() ),
    m_pStaticMeshManager( new CStaticMeshManager() ),
    m_pRenderableObjectsManager( new CRenderableObjectsManager() ),
    m_pAnimatedModelsManager( new CAnimatedModelsManager() ),
    m_pScriptManager( new CScriptManager() ),
    m_pCameraManager( new CCameraManager() ),
    m_pEffectManager( new CEffectManager() ),
	m_pLightManager( new CLightManager() )
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
    CHECKED_DELETE(m_pStaticMeshManager);
    CHECKED_DELETE(m_pRenderableObjectsManager);
    CHECKED_DELETE(m_pAnimatedModelsManager);
    CHECKED_DELETE(m_pTextureManager);
    CHECKED_DELETE(m_pScriptManager);
    CHECKED_DELETE(m_pCameraManager);
    CHECKED_DELETE(m_pEffectManager);
	CHECKED_DELETE(m_pLightManager);
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
    m_pActionManager->Update(deltaTime);
    m_pDebugWindowManager->Update();
    m_pRenderableObjectsManager->Update(deltaTime);
    m_pCameraManager->Update();
}

void CCore::Render()
{
    m_pGraphicsManager->Render();
    m_pDebugWindowManager->Render();
    m_pRenderableObjectsManager->Render();
	m_pLightManager->Render();
    m_pCameraManager->RenderCameras();

    // START: TO DELETE LATER IF IS NOT NECESSARY,
    unsigned int v = m_pRenderableObjectsManager->GetNumVertex();
    unsigned int f = m_pRenderableObjectsManager->GetNumFaces();
    unsigned int d = m_pRenderableObjectsManager->GetNumDraws();
    m_pFontManager->DrawDefaultText(300, 0, Math::CColor(0.0f, 0.0f, 0.0f), "Vertex: %u   Faces: %u   Draws:%u", v, f, d);
    // END: TO DELETE LATER IF IS NOT NECESSARY
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
            else if( TagName == "animated_models" )
            {
                m_AnimatedModelsPath = std::string( TreeNode(i).GetPszProperty("path", "") );
            }
            else if( TagName == "static_meshes" )
            {
                m_StaticMeshesPath = std::string( TreeNode(i).GetPszProperty("path", "") );
            }
            else if( TagName == "renderable_objects" )
            {
                m_RenderableObjectsPath = std::string( TreeNode(i).GetPszProperty("path", "") );
            }
			else if( TagName == "lua" )
            {
                m_LuaRunPath = std::string( TreeNode(i).GetPszProperty("path", "") );
            }
        }
    }
}

void CCore::InitManagers()
{
    m_pGraphicsManager->Init( m_WindowId, m_FullScreenMode, m_ScreenWidth, m_ScreenHeight);
    m_pInputManager->Init( m_WindowId, Math::Vect2i( m_ScreenWidth, m_ScreenHeight ), m_ExclusiveModeInMouse );
    m_pActionManager->Init( m_InputPath, m_pInputManager );
    m_pLanguageManager->SetXmlPaths( m_v_languages );
    m_pLanguageManager->LoadXMLs();
    m_pLanguageManager->SetCurrentLanguage(m_CurrentLanguage);
    m_pFontManager->Init(m_pGraphicsManager);
    m_pFontManager->LoadTTFs(m_FontsPath);
    m_pStaticMeshManager->Load(m_StaticMeshesPath);
    m_pAnimatedModelsManager->Load(m_AnimatedModelsPath);
    m_pRenderableObjectsManager->Load(m_RenderableObjectsPath);

	m_pScriptManager->Initialize();
	m_pScriptManager->RunFile(m_LuaRunPath);

	m_pLightManager->Load("Data/Lights.xml");
}