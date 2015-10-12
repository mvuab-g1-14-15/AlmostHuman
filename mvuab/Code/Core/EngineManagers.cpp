#include "EngineManagers.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

#include "ScriptManager.h"
#include "Texture\TextureManager.h"
#include "Gizmos\GizmosManager.h"
#include "Characters\Enemies\EnemyManager.h"
#include "PhysicsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"
#include "Triggers\TriggerManager.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\Scene.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Cameras\CameraManager.h"
#include "Fonts\FontManager.h"
#include "Language\LanguageManager.h"
#include "Utils\ObjectFactory.h"
#include "GUIManager.h"
#include "Billboard\BillboardManager.h"
#include "Console/Console.h"
#include "WWSoundManager.h"
#include "Timer/CounTDownTimerManager.h"
#include "Cinematics/CinematicManager.h"
#include "EditorsManager.h"
#include "LensFlare\LensFlareManager.h"
#include "Utils\IdManager.h"
#include "Particles\ParticleSystemManager.h"
#include "Utils\BaseUtils.h"
#include "Utils\StringUtils.h"

CEngineManagers::CEngineManagers( const std::string& aPath )

    : m_ManagersPath( aPath )
    , m_pTextureManager( 0 )
    , m_pGraphicsManager( 0 )
    , m_pInputManager( 0 )
    , m_pLanguageManager( 0 )
    , m_pFontManager( 0 )
    , m_pActionManager( 0 )
    , m_pStaticMeshManager( 0 )
    , m_pScene( 0 )
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
    , m_pGizmosManager( 0 )
    , m_pGUIManager( 0 )
    , m_pSoundManager( 0 )
    , m_pCountDownTimerManager( 0 )
    , m_pCinematicManager( 0 )
    , m_pIdManager( 0 )
    , m_pEditorsManager( 0 )
	, m_bGamePause( false )
{
}

CEngineManagers::~CEngineManagers()
{
    Release();
}

void CEngineManagers::Init()
{
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
    ManagerFactory.Register( "scene",
                             Type2Type<CScene>( ) );
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
    ManagerFactory.Register( "trigger_manager",
                             Type2Type<CTriggerManager>( ) );
    ManagerFactory.Register( "gizmos_manager",
                             Type2Type<CGizmosManager>( ) );
    ManagerFactory.Register( "language_manager",
                             Type2Type<CLanguageManager>( ) );
    ManagerFactory.Register( "sound_manager",
                             Type2Type<CWWSoundManager>( ) );
    ManagerFactory.Register( "gui_manager",
                             Type2Type<CGUIManager>( ) );
    ManagerFactory.Register( "console",
                             Type2Type<CConsole>( ) );
    ManagerFactory.Register( "billboard_manager",
                             Type2Type<CBillboardManager>( ) );
    ManagerFactory.Register( "countdowntimer_manager",
                             Type2Type<CCountDownTimerManager>( ) );
    ManagerFactory.Register( "cinematic_manager", Type2Type<CCinematicManager>( ) );
    ManagerFactory.Register( "lens_flares_manager", Type2Type<CLensFlareManager>() );
    ManagerFactory.Register( "id_manager", Type2Type<CIdManager>() );
    ManagerFactory.Register( "editors_manager", Type2Type<CEditorsManager>() );
    ManagerFactory.Register( "particle_system_manager", Type2Type<CParticleSystemManager>() );

    CXMLTreeNode l_File, TreeNode;

    if ( !l_File.LoadAndFindNode( m_ManagersPath.c_str(), "managers", TreeNode ) )
    {
        const std::string& lMsgError = "Error reading the file " + m_ManagersPath;
        FATAL_ERROR( lMsgError.c_str() );
    }
    for ( int i = 0, count = TreeNode.GetNumChildren(); i < count; ++i )
    {
        const CXMLTreeNode& Node = TreeNode( i );
        const std::string& TagName = Node.GetName();
        CManager* Manager = ManagerFactory.Create( TagName.c_str(), Node );

        if ( !Manager )
        {
            LOG_ERROR_APPLICATION( "Manager %s not found in the factory of managers!", TagName.c_str() );
        }
        else
        {
            if ( !AddResource( TagName.c_str() , Manager ) )
                CHECKED_DELETE( Manager );
        }
    }

    m_pConsole          = dynamic_cast<CConsole*>( GetResource( "console" ) );
    m_pScriptManager    = dynamic_cast<CScriptManager*>( GetResource( "script_manager" ) );
    m_pGraphicsManager  = dynamic_cast<CGraphicsManager*>( GetResource( "graphics_manager" ) );
    m_pInputManager     = dynamic_cast<CInputManager*>( GetResource( "input_manager" ) );
    m_pActionManager    = dynamic_cast<CActionManager*>( GetResource( "action_manager" ) );
    m_pFontManager      = dynamic_cast<CFontManager*>( GetResource( "font_manager" ) );
    m_pLanguageManager  = dynamic_cast<CLanguageManager*>( GetResource( "language_manager" ) );
//m_pEnemyManager     = dynamic_cast<CEnemyManager*>( GetResource( "enemy_manager" ) );
    m_pStaticMeshManager = dynamic_cast<CStaticMeshManager*>( GetResource( "static_mesh_manager" ) );
    m_pScene = dynamic_cast<CScene*>( GetResource( "scene" ) );
    m_pRenderableObjectTechniqueManager = dynamic_cast<CRenderableObjectTechniqueManager*>
                                          ( GetResource( "renderable_technique_manager" ) );
    m_pAnimatedModelsManager = dynamic_cast<CAnimatedModelsManager*>( GetResource( "animated_manager" ) );
    m_pTextureManager   = dynamic_cast<CTextureManager*>( GetResource( "texture_manager" ) );
    m_pCameraManager    = dynamic_cast<CCameraManager*>( GetResource( "camera_manager" ) );
    m_pEffectManager    = dynamic_cast<CEffectManager*>( GetResource( "effect_manager" ) );
    m_pLightManager     = dynamic_cast<CLightManager*>( GetResource( "light_manager" ) );
    m_pSceneRendererCommandManager = dynamic_cast<CSceneRendererCommandManager*>
                                     ( GetResource( "scene_render_command_manager" ) );
    m_pPhysicsManager   = dynamic_cast<CPhysicsManager*>( GetResource( "physics_manager" ) );
    m_pTriggerManager   = dynamic_cast<CTriggerManager*>( GetResource( "trigger_manager" ) );
    m_pGizmosManager    = dynamic_cast<CGizmosManager*>( GetResource( "gizmos_manager" ) );
    m_pSoundManager     = dynamic_cast<CWWSoundManager*>( GetResource( "sound_manager" ) );
    m_pGUIManager       = dynamic_cast<CGUIManager*>( GetResource( "gui_manager" ) );
    m_BillboardManager        = dynamic_cast<CBillboardManager*>( GetResource( "billboard_manager" ) );
    m_pCountDownTimerManager  = dynamic_cast<CCountDownTimerManager*>( GetResource( "countdowntimer_manager" ) );
    m_pCinematicManager = dynamic_cast<CCinematicManager*>( GetResource( "cinematic_manager" ) );
    m_pLensFlareManager = dynamic_cast<CLensFlareManager*>( GetResource( "lens_flares_manager" ) );
    m_pIdManager = dynamic_cast<CIdManager*>( GetResource( "id_manager" ) );
    m_pEditorsManager = dynamic_cast<CEditorsManager*>( GetResource( "editors_manager" ) );
    m_pParticleSystemManager = dynamic_cast<CParticleSystemManager*>( GetResource( "particle_system_manager" ) );

#ifdef _DEBUG
    uint32 lSizeElement = GetResourcesCount();
    uint32 lPosition = 0;
    uint32 result;
    std::string lMsg;

    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte;
        ++lItb )
    {
        TIMER_START();
        ( *lItb )->Init();
        result = ++lPosition * 100 / lSizeElement;
        StringUtils::Format( lMsg, "[%u Percentage Completed] Manager %s", result, (*lItb)->GetName().c_str() );
        TIMER_STOP( lMsg.c_str() );
    }
#else
    for( uint32 i = 0, lCount = GetResourcesCount(); i < lCount; ++i )
    {
        m_ResourcesVector[i]->Init();
    }
#endif // _DEBUG
}

void CEngineManagers::Update()
{
    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte;
            ++lItb )
	{
        if( m_bGamePause )
		{
			if( m_pGUIManager == ( *lItb ) || m_pSoundManager == ( *lItb ) )
			{
				( *lItb )->Update();
			}
		}
		else
		{
			( *lItb )->Update();
		}
				
	}
}

void CEngineManagers::Render()
{
    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte;
            ++lItb )
        ( *lItb )->Render();
}

void CEngineManagers::Release()
{

    for ( size_t i = 0 ; i < m_ResourcesVector.size() ; ++i )
    {
        CHECKED_DELETE( m_ResourcesVector[i] )
    }

    /*for ( size_t i = m_ResourcesVector.size() - 1 ; i > 0 ; --i )
    {
        CHECKED_DELETE( m_ResourcesVector[i] )
    }

    CHECKED_DELETE( m_ResourcesVector[0] )*/
    m_ResourcesVector.clear();
    m_ResourcesMap.clear();
	m_bGamePause = false;
}

void CEngineManagers::Reload()
{
    Release();
    Init();
}

CLensFlareManager* CEngineManagers::GetLensFlareManager() const
{
    ASSERT( m_pLensFlareManager, "Null lens flare manager" );
    return m_pLensFlareManager;
}

CWWSoundManager* CEngineManagers::GetSoundManager() const
{
    ASSERT( m_pSoundManager, "Null sound manager" );
    return m_pSoundManager;
}

CEditorsManager* CEngineManagers::GetEditorsManager() const
{
    ASSERT( m_pEditorsManager, "Null editors manager" );
    return m_pEditorsManager;
}

CBillboardManager* CEngineManagers::GetBillboardManager() const
{
    ASSERT( m_BillboardManager, "Null billboard manager" );
    return m_BillboardManager;
}

CGraphicsManager* CEngineManagers::GetGraphicsManager() const
{
    ASSERT( m_pGraphicsManager, "Null graphics manager" );
    return m_pGraphicsManager;
}

CEffectManager* CEngineManagers::GetEffectsManager() const
{
    ASSERT( m_pEffectManager, "Null effects manager" );
    return m_pEffectManager;
}

CInputManager* CEngineManagers::GetInputManager() const
{
    ASSERT( m_pInputManager, "Null input manager" );
    return m_pInputManager;
}

CActionManager* CEngineManagers::GetActionManager() const
{
    ASSERT( m_pActionManager, "Null action manager" );
    return m_pActionManager;
}

CLanguageManager* CEngineManagers::GetLanguageManager() const
{
    ASSERT( m_pLanguageManager, "Null language manager" );
    return m_pLanguageManager;
}

CFontManager* CEngineManagers::GetFontManager() const
{
    ASSERT( m_pFontManager, "Null font manager" );
    return m_pFontManager;
}

CScene*   CEngineManagers::GetSceneManager() const
{
    ASSERT( m_pScene, "Null Scene manager" );
    return m_pScene;
}

CStaticMeshManager* CEngineManagers::GetStaticMeshManager() const
{
    ASSERT( m_pStaticMeshManager, "Null static mesh manager" );
    return m_pStaticMeshManager;
}

CRenderableObjectTechniqueManager* CEngineManagers::GetROTManager() const
{
    ASSERT( m_pRenderableObjectTechniqueManager, "Null Renderable Objects technique manager" );
    return m_pRenderableObjectTechniqueManager;
}

CAnimatedModelsManager* CEngineManagers::GetAnimatedModelsManager() const
{
    ASSERT( m_pAnimatedModelsManager, "Null animated models manager" );
    return m_pAnimatedModelsManager;
}

CSceneRendererCommandManager* CEngineManagers::GetSceneRendererCmdManager() const
{
    ASSERT( m_pSceneRendererCommandManager, "Null scene renderer commands manager" );
    return m_pSceneRendererCommandManager;
}

CCameraManager* CEngineManagers::GetCameraManager() const
{
    ASSERT( m_pCameraManager, "Null camera manager" );
    return m_pCameraManager;
}

CScriptManager* CEngineManagers::GetScriptManager() const
{
    ASSERT( m_pScriptManager, "Null script manager" );
    return m_pScriptManager;
}

CLightManager* CEngineManagers::GetLightManager() const
{
    ASSERT( m_pLightManager, "Null light manager" );
    return m_pLightManager;
}

CPhysicsManager* CEngineManagers::GetPhysicsManager() const
{
    ASSERT( m_pPhysicsManager, "Null physics manager" );
    return m_pPhysicsManager;
}

CEnemyManager* CEngineManagers::GetEnemyManager() const
{
    ASSERT( m_pEnemyManager, "Null enemy manager" );
    return m_pEnemyManager;
}

CTriggerManager* CEngineManagers::GetTriggerManager() const
{
    ASSERT( m_pTriggerManager, "Null trigger manager" );
    return m_pTriggerManager;
}

CGizmosManager* CEngineManagers::GetGizmosManager() const
{
    ASSERT( m_pGizmosManager, "Null gizmo manager" );
    return m_pGizmosManager;
}

CTextureManager* CEngineManagers::GetTextureManager() const
{
    ASSERT( m_pTextureManager, "Null texture manager" );
    return m_pTextureManager;
}

CGUIManager* CEngineManagers::GetGUIManager() const
{
    ASSERT( m_pGUIManager, "Null gui manager" );
    return m_pGUIManager;
}

CCountDownTimerManager* CEngineManagers::GetCountDownTimerManager() const
{
    ASSERT( m_pCountDownTimerManager, "Null CountdownTimer manager" );
    return m_pCountDownTimerManager;
}

CCinematicManager* CEngineManagers::GetCinematicManager() const
{
    ASSERT( m_pCinematicManager, "Null Cinematic manager" );
    return m_pCinematicManager;
}

CIdManager* CEngineManagers::GetIdManager() const
{
    ASSERT( m_pIdManager, "Null id manager" );
    return m_pIdManager;
}

CParticleSystemManager* CEngineManagers::GetParticleSystemManager() const
{
    ASSERT( m_pParticleSystemManager, "Null Particle System manager" );
    return m_pParticleSystemManager;
}