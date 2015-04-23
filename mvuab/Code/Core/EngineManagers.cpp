#include "EngineManagers.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

#include "ScriptManager.h"
#include "Texture\TextureManager.h"
#include "Gizmos\GizmosManager.h"
#include "PhysicsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Effects\EffectManager.h"
#include "Particles\ParticleManager.h"
#include "Lights\LightManager.h"
#include "Triggers\TriggerManager.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Cameras\CameraManager.h"
#include "Fonts\FontManager.h"
#include "Language\LanguageManager.h"
#include "Utils\ObjectFactory.h"
#include "GUIManager.h"
#include "Console/Console.h"
#include "WWSoundManager.h"

CEngineManagers::CEngineManagers( const std::string& aPath )
    : m_ManagersPath( aPath )
    , m_pTextureManager( 0 )
    , m_pGraphicsManager( 0 )
    , m_pInputManager( 0 )
    , m_pLanguageManager( 0 )
    , m_pFontManager( 0 )
    , m_pActionManager( 0 )
    , m_pStaticMeshManager( 0 )
    , m_pRenderableObjectsLayersManager( 0 )
    , m_pRenderableObjectTechniqueManager( 0 )
    , m_pAnimatedModelsManager( 0 )
    , m_pScriptManager( 0 )
    , m_pCameraManager( 0 )
    , m_pEffectManager( 0 )
    , m_pLightManager( 0 )
    , m_pSceneRendererCommandManager( 0 )
    , m_pPhysicsManager( 0 )
    , m_pTriggerManager( 0 )
    , m_pParticleManager( 0 )
    , m_pGizmosManager( 0 )
    , m_pGUIManager( 0 )
    , m_pSoundManager( 0 )
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
    ManagerFactory.Register( "trigger_manager",
                             Type2Type<CTriggerManager>( ) );
    ManagerFactory.Register( "particle_manager",
                             Type2Type<CParticleManager>( ) );
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
    CXMLTreeNode l_File;

    if ( !l_File.LoadFile( m_ManagersPath.c_str() ) )
    {
        const std::string& lMsgError = "Error reading the file " + m_ManagersPath;
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

            CManager* Manager = ManagerFactory.Create( TagName.c_str(), TreeNode( i ) );

            if ( !Manager )
                LOG_ERROR_APPLICATION( "Manager %s not found in the factory of managers!", TagName.c_str() );
            else
            {
                if ( !AddResource( TagName.c_str() , Manager ) )
                    CHECKED_DELETE( Manager );
            }
        }
    }

    m_pScriptManager    = dynamic_cast<CScriptManager*>( GetResource( "script_manager" ) );
    m_pGraphicsManager  = dynamic_cast<CGraphicsManager*>( GetResource( "graphics_manager" ) );
    m_pInputManager     = dynamic_cast<CInputManager*>( GetResource( "input_manager" ) );
    m_pActionManager    = dynamic_cast<CActionManager*>( GetResource( "action_manager" ) );
    m_pFontManager      = dynamic_cast<CFontManager*>( GetResource( "font_manager" ) );
    m_pLanguageManager  = dynamic_cast<CLanguageManager*>( GetResource( "language_manager" ) );
    m_pStaticMeshManager = dynamic_cast<CStaticMeshManager*>( GetResource( "static_mesh_manager" ) );
    m_pRenderableObjectsLayersManager = dynamic_cast<CRenderableObjectsLayersManager*>
                                        ( GetResource( "renderable_manager_layer" ) );
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
    m_pParticleManager  = dynamic_cast<CParticleManager*>( GetResource( "particle_manager" ) );
    m_pGizmosManager    = dynamic_cast<CGizmosManager*>( GetResource( "gizmos_manager" ) );
    m_pSoundManager     = dynamic_cast<CWWSoundManager*>( GetResource( "sound_manager" ) );
    m_pGUIManager       = dynamic_cast<CGUIManager*>( GetResource( "gui_manager" ) );

    //
    // Init managers
    //
    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte; ++lItb )
        ( *lItb )->Init();
}

void CEngineManagers::Update()
{
    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte; ++lItb )
        ( *lItb )->Update();
}

void CEngineManagers::Render()
{
    for ( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end() ; lItb != lIte; ++lItb )
        ( *lItb )->Render();
}

void CEngineManagers::Release()
{
    for ( size_t i = m_ResourcesVector.size() - 1 ; i = 0 ; --i )
    {
        CHECKED_DELETE( m_ResourcesVector[i] )
    }
}

CWWSoundManager* CEngineManagers::GetSoundManager() const
{
    ASSERT( m_pSoundManager, "Null sound manager" );
    return m_pSoundManager;
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

CRenderableObjectsLayersManager*  CEngineManagers::GetROLManager() const
{
    ASSERT( m_pRenderableObjectsLayersManager, "Null Renderable Objects Layers manager" );
    return m_pRenderableObjectsLayersManager;
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

CTriggerManager* CEngineManagers::GetTriggerManager() const
{
    ASSERT( m_pTriggerManager, "Null trigger manager" );
    return m_pTriggerManager;
}

CParticleManager* CEngineManagers::GetParticleManager() const
{
    ASSERT( m_pParticleManager, "Null particle manager manager" );
    return m_pParticleManager;
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
