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
#include "Gizmos\GizmosManager.h"
#include "EngineConfig.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

#include <iostream>

CCore::CCore()
  : m_pTextureManager( 0 )
  , m_pGraphicsManager( 0 )
  , m_pInputManager( 0 )
  , m_pLanguageManager( 0 )
  , m_pFontManager( 0 )
  , m_pActionManager( 0 )
  , m_pStaticMeshManager( 0 )
  , m_pRenderableObjectsManager( 0 )
  , m_pRenderableObjectsLayersManager( 0 )
  , m_pRenderableObjectTechniqueManager( 0 )
  , m_pAnimatedModelsManager( 0 )
  , m_pScriptManager( 0 )
  , m_pCameraManager( 0 )
  , m_pEffectManager( 0 )
  , m_pLightManager( 0 )
  , m_pSceneRendererCommandManager( 0 )
  , m_pPhysicsManager( 0 )
  , m_pEnemyManager( 0 )
  , m_pTriggerManager( 0 )
  , m_pTimer( 0 )
  , m_pConsole( 0 )
  , m_pBillboard( 0 )
  , m_pParticleManager( 0 )
  , m_pGizmosManager( 0 )
{
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
  CHECKED_DELETE( m_pGizmosManager );
  CHECKED_DELETE( m_pConsole );
}

void CCore::Init( const std::string& aConfigPath, HWND aWindowId )
{
  // Obtain the path of the configuration file and parse it
  m_WindowId = aWindowId;

  CreateManagers();
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

  m_pGizmosManager->Update();

  m_pBillboard->Update();

  m_pRenderableObjectsLayersManager->GetResource( "solid" )->Update();

  if ( m_pActionManager->DoAction( "ClearConsole" ) )
  {
    if ( m_pConsole )
      m_pConsole->Clear();
  }
}

void CCore::Render()
{
}
void CCore::CreateManagers()
{
  CEngineConfig* lEngineConfig = CEngineConfig::GetSingletonPtr();

  if ( lEngineConfig->GetEnableConsole() )
    m_pConsole = new CConsole( TRUE );

  m_pTextureManager = new CTextureManager( );
  m_pGraphicsManager = new CGraphicsManager( );
  m_pInputManager = new CInputManager( );
  m_pLanguageManager = new CLanguageManager( );
  m_pFontManager = new CFontManager( );
  m_pActionManager = new CActionManager( );
  m_pStaticMeshManager = new CStaticMeshManager( );
  m_pRenderableObjectsManager = new CRenderableObjectsManager( );
  m_pRenderableObjectsLayersManager = new CRenderableObjectsLayersManager( );
  m_pRenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager( );
  m_pAnimatedModelsManager = new CAnimatedModelsManager( );
  m_pScriptManager = new CScriptManager( );
  m_pCameraManager = new CCameraManager( );
  m_pEffectManager = new CEffectManager( );
  m_pLightManager = new CLightManager( );
  m_pSceneRendererCommandManager = new CSceneRendererCommandManager( );
  m_pPhysicsManager = new CPhysicsManager( );
  m_pEnemyManager = new CEnemyManager( );
  m_pTriggerManager = new CTriggerManager( );
  m_pTimer = new CTimer( 30 );
  m_pBillboard = new CBillboard( );
  m_pParticleManager = new CParticleManager();
  m_pGizmosManager = new CGizmosManager();
}

void CCore::InitManagers()
{
  CEngineConfig* l_EngineConfig = CEngineConfig::GetSingletonPtr();

  if ( l_EngineConfig->GetEnableConsole() )
  {
    m_pConsole->RedirectToConsole( 0 );
    m_pConsole->SetNumberOfLines( 800 );
    m_pConsole->SetNumberOfColumns( 132 );
    m_pConsole->SetMaxLinesInWindow( 25 );
    WORD x = m_pConsole->SetAttributes( FOREGROUND_BLUE, 2 );
    m_pConsole->SetAttributes( x );
    m_pConsole->SetFullSize();
  }

  m_pGraphicsManager->Init( m_WindowId, l_EngineConfig );
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
  m_pGizmosManager->Init();
}

void CCore::Trace( const std::string& TraceStr )
{
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
  SetConsoleTextAttribute( hConsole,
                           FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
  std::cout << TraceStr << std::endl << std::endl;
}