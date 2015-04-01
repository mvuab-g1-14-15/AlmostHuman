#include "Core.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "ScriptManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"
#include "Timer\Timer.h"
#include "Console\Console.h"
#include "PhysicsManager.h"
#include "Triggers\TriggerManager.h"
#include "Actor\PhysicActor.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Billboard\Billboard.h"
#include "Particles\ParticleManager.h"
#include "EngineConfig.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

#include <iostream>

CCore::CCore() :
  m_CurrentLanguage( "" ),
  m_ConfigPath( "" ),
  m_GUIPath( "" ),
  m_SoundPath( "" ),
  m_FontsPath( "" ),
  m_InputPath( "" ),
  m_StaticMeshesPath( "" ),
  m_RenderableObjectsPath( "" ),
  m_RenderableObjectTechniquePath( "" ),
  m_AnimatedModelsPath( "" ),
  m_LuaRunPath( "" ),
  m_EffectsPath( "" ),
  m_LightsPath( "" ),
  m_SceneRendererCommandPath( "" ),
  m_TriggersPath( "" ),
  m_ScreenWidth( 800 ),
  m_ScreenHeight( 600 ),
  m_WindowXPos( 0 ),
  m_WindowYPos( 0 ),
  m_FullScreenMode( false ),
  m_ExclusiveModeInMouse( false ),
  m_DrawPointerMouse( false ),
  m_pTextureManager( new CTextureManager() ),
  m_pGraphicsManager( new CGraphicsManager() ),
  m_pInputManager( new CInputManager() ),
  m_pLanguageManager( new CLanguageManager() ),
  m_pFontManager( new CFontManager() ),
  m_pActionManager( new CActionManager() ),
  m_pStaticMeshManager( new CStaticMeshManager() ),
  m_pRenderableObjectsManager( new CRenderableObjectsManager() ),
  m_pRenderableObjectsLayersManager( new CRenderableObjectsLayersManager() ),
  m_pRenderableObjectTechniqueManager( new CRenderableObjectTechniqueManager() ),
  m_pAnimatedModelsManager( new CAnimatedModelsManager() ),
  m_pScriptManager( new CScriptManager() ),
  m_pCameraManager( new CCameraManager() ),
  m_pEffectManager( new CEffectManager() ),
  m_pLightManager( new CLightManager() ),
  m_pSceneRendererCommandManager( new CSceneRendererCommandManager() ),
  m_pPhysicsManager( new CPhysicsManager() ),
  m_pEnemyManager( new CEnemyManager() ),
  m_pTriggerManager( new CTriggerManager() ),
  m_pTimer( new CTimer( 30 ) ),
  m_pConsole( new CConsole( TRUE ) ),
  m_pBillboard( new CBillboard() ),
  m_pParticleManager( new CParticleManager() )
{

#ifdef _DEBUG
  m_pConsole->RedirectToConsole( 0 );
  m_pConsole->SetNumberOfLines( 800 );
  m_pConsole->SetNumberOfColumns( 132 );
  m_pConsole->SetMaxLinesInWindow( 25 );
  WORD x = m_pConsole->SetAttributes( FOREGROUND_BLUE | BACKGROUND_GREEN, 2 );
  m_pConsole->SetAttributes( x );
  m_pConsole->SetFullSize();
#endif
}

CCore::~CCore()
{
  CHECKED_DELETE( m_pScriptManager );
  CHECKED_DELETE( m_pGraphicsManager );
  CHECKED_DELETE( m_pInputManager );
  CHECKED_DELETE( m_pActionManager );
  CHECKED_DELETE( m_pFontManager );
  CHECKED_DELETE( m_pLanguageManager );
  CHECKED_DELETE( m_pEnemyManager );
  CHECKED_DELETE( m_pStaticMeshManager );
  CHECKED_DELETE( m_pRenderableObjectsManager );
  CHECKED_DELETE( m_pRenderableObjectsLayersManager );
  CHECKED_DELETE( m_pRenderableObjectTechniqueManager );
  CHECKED_DELETE( m_pAnimatedModelsManager );
  CHECKED_DELETE( m_pTextureManager );
  CHECKED_DELETE( m_pCameraManager );
  CHECKED_DELETE( m_pEffectManager );
  CHECKED_DELETE( m_pLightManager );
  CHECKED_DELETE( m_pSceneRendererCommandManager );
  CHECKED_DELETE( m_pTimer );
  CHECKED_DELETE( m_pConsole );
  CHECKED_DELETE( m_pPhysicsManager );
  CHECKED_DELETE( m_pTriggerManager );
  CHECKED_DELETE( m_pBillboard );
  CHECKED_DELETE( m_pParticleManager );
}

void CCore::Init( const std::string& aConfigPath, HWND aWindowId )
{
  // Obtain the path of the configuration file and parse it
  m_ConfigPath = aConfigPath;
  m_WindowId = aWindowId;
  //LoadXml();
  // Init the managers
  InitManagers();
}

void CCore::Update()
{
  m_pTimer->Update();

  m_pInputManager->Update();
  m_pActionManager->Update();

  m_pPhysicsManager->Update( deltaTime );
  m_pEnemyManager->Update();

  m_pCameraManager->Update();

  m_pParticleManager->Update( deltaTime );
  m_pBillboard->Update();

  m_pRenderableObjectsLayersManager->GetResource( "solid" )->Update();

  if ( m_pActionManager->DoAction( "ClearConsole" ) )
    m_pConsole->Clear();
}

void CCore::Render()
{
}

void CCore::LoadXml()
{
  //if ( m_ConfigPath == "" )
  //  return;

  //CXMLTreeNode l_File;

  //if ( !l_File.LoadFile( m_ConfigPath.c_str() ) )
  //{
  //  std::string err = "ERROR reading the file " + m_ConfigPath;
  //  MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
  //  exit( EXIT_FAILURE );
  //}

  //CXMLTreeNode  TreeNode = l_File["config"];

  //if ( TreeNode.Exists() )
  //{
  //  int count = TreeNode.GetNumChildren();

  //  for ( int i = 0; i < count; ++i )
  //  {
  //    std::string TagName = TreeNode( i ).GetName();

  //    if ( TagName == "screen_resolution" )
  //    {
  //      m_ScreenWidth = TreeNode( i ).GetIntProperty( "width" );
  //      m_ScreenHeight = TreeNode( i ).GetIntProperty( "height" );
  //    }
  //    else if ( TagName == "window_position" )
  //    {
  //      m_WindowXPos = TreeNode( i ).GetIntProperty( "x_pos", 0 );
  //      m_WindowYPos = TreeNode( i ).GetIntProperty( "y_pos", 0 );
  //    }
  //    else if ( TagName == "render_mode" )
  //      m_FullScreenMode = TreeNode( i ).GetBoolProperty( "fullscreen_mode", false );
  //    else if ( TagName == "mouse" )
  //    {
  //      m_ExclusiveModeInMouse = TreeNode(
  //                                 i ).GetBoolProperty( "exclusive_mode_in_mouse", false );
  //      m_DrawPointerMouse = TreeNode( i ).GetBoolProperty( "draw_pointer_mouse",
  //                           false );
  //    }
  //    else if ( TagName == "GUI" )
  //      m_GUIPath = std::string( TreeNode( i ).GetPszProperty( "init_gui_path", "" ) );
  //    else if ( TagName == "sound" )
  //      m_SoundPath = std::string( TreeNode( i ).GetPszProperty( "init_sound_path",
  //                                 "" ) );
  //    else if ( TagName == "fonts" )
  //      m_FontsPath = std::string( TreeNode( i ).GetPszProperty( "fonts_path", "" ) );
  //    else if ( TagName == "input" )
  //      m_InputPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "languages" )
  //    {
  //      m_CurrentLanguage = std::string( TreeNode(
  //                                         i ).GetPszProperty( "current_language", "" ) );
  //      CXMLTreeNode  SubTreeNode = l_File["languages"];

  //      if ( SubTreeNode.Exists() )
  //      {
  //        int countLan = SubTreeNode.GetNumChildren();

  //        for ( int lans = 0; lans < countLan; ++lans )
  //        {
  //          std::string TagName = SubTreeNode( lans ).GetName();

  //          if ( TagName == "language" )
  //            m_v_languages.push_back( std::string( SubTreeNode(
  //                                                    lans ).GetPszProperty( "path", "" ) ) );
  //        }
  //      }

  //      m_FontsPath = std::string( TreeNode( i ).GetPszProperty( "fonts_path", "" ) );
  //    }
  //    else if ( TagName == "animated_models" )
  //      m_AnimatedModelsPath = std::string( TreeNode( i ).GetPszProperty( "path",
  //                                          "" ) );
  //    else if ( TagName == "static_meshes" )
  //      m_StaticMeshesPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "renderable_objects" )
  //      m_RenderableObjectsPath = std::string( TreeNode( i ).GetPszProperty( "path",
  //                                             "" ) );
  //    else if ( TagName == "renderable_object_technique" )
  //      m_RenderableObjectTechniquePath = std::string( TreeNode(
  //                                          i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "lua" )
  //      m_LuaRunPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "effects" )
  //      m_EffectsPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "lights" )
  //      m_LightsPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "scene_renderer_commands" )
  //      m_SceneRendererCommandPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //    else if ( TagName == "triggers" )
  //      m_TriggersPath = std::string( TreeNode( i ).GetPszProperty( "path", "" ) );
  //  }
  //}
}

void CCore::InitManagers()
{
  CEngineConfig* l_EngineConfig = CEngineConfig::GetSingletonPtr();
  m_pGraphicsManager->Init( m_WindowId, l_EngineConfig->GetFullScreenMode(), l_EngineConfig->GetScreenWidth(),
                            l_EngineConfig->GetScreenHeight() );
  m_pEffectManager->Load( l_EngineConfig->GetEffectsPath() );
  m_pInputManager->Init( m_WindowId, Math::Vect2i( l_EngineConfig->GetScreenWidth(),
                         l_EngineConfig->GetScreenHeight() ), l_EngineConfig->GetExclusiveModeInMouse() );
  m_pActionManager->Init( l_EngineConfig->GetInputPath(), m_pInputManager );
  m_pLanguageManager->SetXmlPaths( l_EngineConfig->GetLanguages() );
  m_pLanguageManager->LoadXMLs();
  m_pLanguageManager->SetCurrentLanguage( l_EngineConfig->GetCurrentLanguage() );
  m_pFontManager->Init( m_pGraphicsManager );
  m_pFontManager->LoadTTFs( l_EngineConfig->GetFontsPath() );
  m_pRenderableObjectTechniqueManager->Load( l_EngineConfig->GetRenderableObjectTechniquePath() );
  m_pStaticMeshManager->Load( l_EngineConfig->GetStaticMeshesPath() );
  m_pAnimatedModelsManager->Load( l_EngineConfig->GetAnimatedModelsPath() );
  m_pRenderableObjectsLayersManager->Load( l_EngineConfig->GetRenderableObjectsPath() );
  m_pSceneRendererCommandManager->Load( l_EngineConfig->GetSceneRendererCommandPath() );
  //m_pRenderableObjectsManager->Load(m_RenderableObjectsPath);
  m_pCameraManager->Init();
  m_pCameraManager->NewCamera( CCamera::FirstPerson, "TestProcessCam",
                               Math::Vect3f( 15.0f, 2.0f, 0.0f ),
                               Math::Vect3f( 0.0f, 2.0f, 0.0f ) );
  m_pCameraManager->NewCamera( CCamera::Free, "FreeCam",
                               Math::Vect3f( 12.28f, -16.75f, 12.75f ),
                               Math::Vect3f( 0.0f, 2.0f, 0.0f ) );
  m_pCameraManager->NewCamera( CCamera::FirstPerson, "Plane",
                               Math::Vect3f( 0.0f, 15.0f, 0.0f ),
                               Math::Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pCameraManager->SetCurrentCamera( "TestProcessCam" );
  m_pScriptManager->Initialize();
  m_pScriptManager->Load( l_EngineConfig->GetLuaRunPath() );
  m_pLightManager->Load( l_EngineConfig->GetLightsPath() );
  m_pPhysicsManager->Init();
  m_pEnemyManager->Init( l_EngineConfig->GetEnemiesPath() );
  m_pTriggerManager->LoadXML( l_EngineConfig->GetTriggersPath() );

  m_pBillboard->Init( Math::Vect3f( 0, 2, 1 ), 2, l_EngineConfig->GetBillboardTexturePath() );
  m_pParticleManager->Init( l_EngineConfig->GetParticlesPath() );
}

void CCore::Trace( const std::string& TraceStr )
{
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
  SetConsoleTextAttribute( hConsole,
                           FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
  std::cout << TraceStr << std::endl << std::endl;
}