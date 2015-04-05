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
class CStaticMeshManager;
class CRenderableObjectsManager;
class CRenderableObjectsLayersManager;
class CRenderableObjectTechniqueManager;
class CAnimatedModelsManager;
class CScriptManager;
class CCameraManager;
class CEffectManager;
class CLightManager;
class CTimer;
class CConsole;
class CSceneRendererCommandManager;
class CPhysicsManager;
class CEnemyManager;
class CTriggerManager;
class CBillboard;
class CParticleManager;
class CGizmosManager;

class CCore : public CSingleton<CCore>
{
    private:

        // Configuration
        HWND                                m_WindowId;

        // Managers
        CGraphicsManager*                   m_pGraphicsManager;
        CInputManager*                      m_pInputManager;
        CLanguageManager*                   m_pLanguageManager;
        CFontManager*                       m_pFontManager;
        CActionManager*                     m_pActionManager;
        CStaticMeshManager*                 m_pStaticMeshManager;
        CRenderableObjectsManager*          m_pRenderableObjectsManager;
        CRenderableObjectsLayersManager*    m_pRenderableObjectsLayersManager;
        CRenderableObjectTechniqueManager*  m_pRenderableObjectTechniqueManager;
        CAnimatedModelsManager*             m_pAnimatedModelsManager;
        CTextureManager*                    m_pTextureManager;
        CScriptManager*                     m_pScriptManager;
        CCameraManager*                     m_pCameraManager;
        CEffectManager*                     m_pEffectManager;
        CLightManager*                      m_pLightManager;
        CSceneRendererCommandManager*       m_pSceneRendererCommandManager;
        CEnemyManager*                      m_pEnemyManager;
        CPhysicsManager*                    m_pPhysicsManager;
        CTriggerManager*                    m_pTriggerManager;
        CTimer*                             m_pTimer;
        CConsole*                           m_pConsole;
        CBillboard*                         m_pBillboard;
        CParticleManager*                   m_pParticleManager;
        CGizmosManager*                     m_pGizmosManager;
        void CreateManagers();
        void InitManagers();

    public:
        CCore();
        ~CCore();

        void Init( const std::string& aConfigPath, HWND aWindowId );
        void Update();
        void Render();

        // Methods that trace Lua
        void Trace( const std::string& TraceStr );


        // Getters and setters
        GET_SET_PTR( CInputManager, InputManager );
        GET_SET_PTR( CGraphicsManager, GraphicsManager );
        GET_SET_PTR( CFontManager, FontManager );
        GET_SET_PTR( CLanguageManager, LanguageManager );
        GET_SET_PTR( CActionManager, ActionManager );
        GET_SET_PTR( CStaticMeshManager, StaticMeshManager );
        GET_SET_PTR( CRenderableObjectsManager, RenderableObjectsManager );
        GET_SET_PTR( CRenderableObjectsLayersManager, RenderableObjectsLayersManager );
        GET_SET_PTR( CRenderableObjectTechniqueManager,
                     RenderableObjectTechniqueManager );
        GET_SET_PTR( CAnimatedModelsManager, AnimatedModelsManager );
        GET_SET_PTR( CTextureManager, TextureManager );
        GET_SET_PTR( CScriptManager, ScriptManager );
        GET_SET_PTR( CCameraManager, CameraManager );
        GET_SET_PTR( CEffectManager, EffectManager );
        GET_SET_PTR( CLightManager, LightManager );
        GET_SET_PTR( CSceneRendererCommandManager, SceneRendererCommandManager );
        GET_SET_PTR( CPhysicsManager, PhysicsManager );
        GET_SET_PTR( CEnemyManager, EnemyManager );
        GET_SET_PTR( CTriggerManager, TriggerManager );
        GET_SET_PTR( CTimer, Timer );
        GET_SET_PTR( CBillboard, Billboard );
        GET_SET_PTR( CParticleManager, ParticleManager );
        GET_SET_PTR( CGizmosManager, GizmosManager);
};

#endif // GRAPHICS_MANAGER_H