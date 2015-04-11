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
  : m_pTimer( 0 )
  , m_pConsole( 0 )
{
}

CCore::~CCore()
{
  Destroy();
  CHECKED_DELETE( m_pTimer );
  CHECKED_DELETE( m_pConsole );
}

void CCore::Init()
{
  // Obtain the path of the configuration file and parse it
  CreateManagers();
  InitManagers();
}

void CCore::Update()
{
  TVectorResources::iterator it = m_ResourcesVector.begin(),
                              it_end = m_ResourcesVector.end();
  for(;it!=it_end;++it)
        (*it)->Update();
  m_pTimer->Update();

  if ( ActionManagerInstance->DoAction( "ClearConsole" ) )
  {
    if ( m_pConsole )
      m_pConsole->Clear();
  }
}

void CCore::Render()
{
    SRCMInstance->Execute();
}
void CCore::CreateManagers()
{
  if ( EngineConfigInstance->GetEnableConsole() )
    m_pConsole = new CConsole( TRUE );
  m_pTimer = new CTimer( 30 );

  ObjectFactory1<CManager, CXMLTreeNode, std::string > ManagerFactory;
  // Register all the commands with the object factory class
  ManagerFactory.Register( "texture_manager",
                            Type2Type<CTextureManager>( ) );
  ManagerFactory.Register( "graphics_manager", 
                            Type2Type<CGraphicsManager>( ) );
  ManagerFactory.Register( "input_manager",
                           Type2Type<CInputManager>( ) );
  ManagerFactory.Register( "languague_manager",
                           Type2Type<CLanguageManager>( ) );
  ManagerFactory.Register( "font_manager",
                           Type2Type<CFontManager>( ) );
  ManagerFactory.Register( "action_manager",
                            Type2Type<CActionManager>( ) );
  ManagerFactory.Register( "static_mesh_manager",
                            Type2Type<CStaticMeshManager>( ) );
  ManagerFactory.Register( "effect_manager",
                           Type2Type<CEffectManager>( ) );
  ManagerFactory.Register( "renderable_manager_layer",
                           Type2Type<CRenderableObjectsLayersManager>( ) );
  ManagerFactory.Register( "renderable_technique_manager",
                           Type2Type<CRenderableObjectTechniqueManager>( ) );
  ManagerFactory.Register( "animated_manager",
                           Type2Type<CAnimatedModelsManager>( ) );
  ManagerFactory.Register( "script_manager",
                           Type2Type<CScriptManager>( ) );
  ManagerFactory.Register( "camera_manager",
                           Type2Type<CCameraManager>( ) );
  ManagerFactory.Register( "light_manager",
                           Type2Type<CLightManager>( ) );
  ManagerFactory.Register( "scene_render_command_manager",
                           Type2Type<CSceneRendererCommandManager>( ) );
  ManagerFactory.Register( "physics_manager",
                           Type2Type<CPhysicsManager>( ) );
  ManagerFactory.Register( "enemy_manager",
                           Type2Type<CEnemyManager>( ) );
  ManagerFactory.Register( "trigger_manager",
                           Type2Type<CTriggerManager>( ) );
  ManagerFactory.Register( "billboard",
                           Type2Type<CBillboard>( ) );
  ManagerFactory.Register( "particle_manager",
                           Type2Type<CParticleManager>( ) );
  ManagerFactory.Register( "gizmos_manager",
                           Type2Type<CGizmosManager>( ) );
  CXMLTreeNode l_File;
  //TODO RAUL
  std::string l_ManagerPath= EngineConfigInstance->GetManagersPath();

  if ( !l_File.LoadFile( l_ManagerPath.c_str() ) )
  {
    const std::string& lMsgError = "Error reading the file " + l_ManagerPath;
    FATAL_ERROR( lMsgError.c_str() );
  }

  
  CXMLTreeNode  TreeNode = l_File["managers"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      const std::string& TagName = TreeNode( i ).GetName();

      if ( TagName == "comment" )
        continue;

      CManager* Manager = 0;

      Manager = ManagerFactory.Create( TagName.c_str(), TreeNode( i ) );

      if ( !Manager )
        LOG_ERROR_APPLICATION( "Manager %s not found in the factory of managers!", TagName.c_str() );
      else
      {
        if ( !AddResource( TagName.c_str() , Manager ) )
          CHECKED_DELETE( Manager );
      }
    }
  }
}

void CCore::InitManagers()
{
  CEngineConfig* l_EngineConfig = EngineConfigInstance;

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

  TVectorResources::iterator it = m_ResourcesVector.begin(),
                             it_end = m_ResourcesVector.end();
  for(;it!=it_end;++it)
      (*it)->Init();
}

void CCore::Trace( const std::string& TraceStr )
{
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
  SetConsoleTextAttribute( hConsole,
                           FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
  std::cout << TraceStr << std::endl << std::endl;
}