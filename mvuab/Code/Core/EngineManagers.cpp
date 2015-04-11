#include "EngineManagers.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Core.h"
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
#include "Particles\ParticleManager.h"
#include "Billboard\Billboard.h"
#include "Lights\LightManager.h"
#include "Triggers\TriggerManager.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Cameras\CameraManager.h"
#include "Fonts\FontManager.h"
#include "Language\LanguageManager.h"

CEngineManagers::CEngineManagers()
   : m_pTextureManager( 0 )
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
  , m_pEnemyManager( 0 )
  , m_pTriggerManager( 0 )
  , m_pBillboard( 0 )
  , m_pParticleManager( 0 )
  , m_pGizmosManager( 0 )
{  
}  

CEngineManagers::~CEngineManagers()
{
 /* CHECKED_DELETE( m_pScriptManager );
  CHECKED_DELETE( m_pGraphicsManager );
  CHECKED_DELETE( m_pInputManager );
  CHECKED_DELETE( m_pActionManager );
  CHECKED_DELETE( m_pFontManager );
  CHECKED_DELETE( m_pLanguageManager );
  CHECKED_DELETE( m_pEnemyManager );
  CHECKED_DELETE( m_pStaticMeshManager );
  CHECKED_DELETE( m_pRenderableObjectsLayersManager );
  CHECKED_DELETE( m_pRenderableObjectTechniqueManager );
  CHECKED_DELETE( m_pAnimatedModelsManager );
  CHECKED_DELETE( m_pTextureManager );
  CHECKED_DELETE( m_pCameraManager );
  CHECKED_DELETE( m_pEffectManager );
  CHECKED_DELETE( m_pLightManager );
  CHECKED_DELETE( m_pSceneRendererCommandManager );
  CHECKED_DELETE( m_pPhysicsManager );
  CHECKED_DELETE( m_pTriggerManager );
  CHECKED_DELETE( m_pBillboard );
  CHECKED_DELETE( m_pParticleManager );
  CHECKED_DELETE( m_pGizmosManager );*/
}

void CEngineManagers::Init()
{
    m_pScriptManager = dynamic_cast<CScriptManager*>(CCore::GetSingletonPtr()->GetResource("script_manager"));
    m_pGraphicsManager = dynamic_cast<CGraphicsManager*>(CCore::GetSingletonPtr()->GetResource("graphics_manager"));
    m_pInputManager = dynamic_cast<CInputManager*>(CCore::GetSingletonPtr()->GetResource("input_manager"));
    m_pActionManager = dynamic_cast<CActionManager*>(CCore::GetSingletonPtr()->GetResource("action_manager"));
    m_pFontManager = dynamic_cast<CFontManager*>(CCore::GetSingletonPtr()->GetResource("font_manager"));
    m_pLanguageManager = dynamic_cast<CLanguageManager*>(CCore::GetSingletonPtr()->GetResource("language_manager"));
    m_pEnemyManager = dynamic_cast<CEnemyManager*>(CCore::GetSingletonPtr()->GetResource("enemy_manager"));
    m_pStaticMeshManager = dynamic_cast<CStaticMeshManager*>(CCore::GetSingletonPtr()->GetResource("static_mesh_manager"));
    m_pRenderableObjectsLayersManager = dynamic_cast<CRenderableObjectsLayersManager*>(CCore::GetSingletonPtr()->GetResource("renderable_manager_layer"));
    m_pRenderableObjectTechniqueManager = dynamic_cast<CRenderableObjectTechniqueManager*>(CCore::GetSingletonPtr()->GetResource("renderable_technique_manager"));
    m_pAnimatedModelsManager = dynamic_cast<CAnimatedModelsManager*>(CCore::GetSingletonPtr()->GetResource("animated_manager"));
    m_pTextureManager = dynamic_cast<CTextureManager*>(CCore::GetSingletonPtr()->GetResource("texture_manager"));
    m_pCameraManager = dynamic_cast<CCameraManager*>(CCore::GetSingletonPtr()->GetResource("camera_manager"));
    m_pEffectManager = dynamic_cast<CEffectManager*>(CCore::GetSingletonPtr()->GetResource("effect_manager"));
    m_pLightManager = dynamic_cast<CLightManager*>(CCore::GetSingletonPtr()->GetResource("light_manager"));
    m_pSceneRendererCommandManager = dynamic_cast<CSceneRendererCommandManager*>(CCore::GetSingletonPtr()->GetResource("scene_render_command_manager"));
    m_pPhysicsManager = dynamic_cast<CPhysicsManager*>(CCore::GetSingletonPtr()->GetResource("physics_manager"));
    m_pTriggerManager = dynamic_cast<CTriggerManager*>(CCore::GetSingletonPtr()->GetResource("trigger_manager"));
    m_pBillboard = dynamic_cast<CBillboard*>(CCore::GetSingletonPtr()->GetResource("billboard"));
    m_pParticleManager = dynamic_cast<CParticleManager*>(CCore::GetSingletonPtr()->GetResource("particle_manager"));
    m_pGizmosManager = dynamic_cast<CGizmosManager*>(CCore::GetSingletonPtr()->GetResource("gizmos_manager"));
}