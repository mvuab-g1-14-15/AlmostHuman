#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "Types.h"
#include <assert.h>
#include "Logger\Logger.h"
#include <Windows.h>

// MACROS
#define GET_SET(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(TYPE PARAMETER_NAME) \
    { \
        m_##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    TYPE Get##PARAMETER_NAME() \
    { \
        return m_##PARAMETER_NAME; \
    }

#define GET_SET_PTR(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(TYPE##* PARAMETER_NAME) \
    { \
        m_p##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    TYPE##* Get##PARAMETER_NAME() \
    { \
        return m_p##PARAMETER_NAME; \
    }

#define GET_SET_REF(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(const TYPE &##PARAMETER_NAME) \
    { \
        m_##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    const TYPE & Get##PARAMETER_NAME() const \
    { \
        return m_##PARAMETER_NAME; \
    }

#define GET_SET_ARRAY(TYPE, PARAMETER_NAME, SIZE) \
  void Set##PARAMETER_NAME(TYPE PARAMETER_NAME##[]) \
    { \
      for (int i = 0; i < SIZE; ++i)\
      {\
        m_##PARAMETER_NAME##[i] = PARAMETER_NAME##[i];\
      }\
    } \
    TYPE##* Get##PARAMETER_NAME() \
    { \
        return m_##PARAMETER_NAME; \
    }
#define DIRECTINPUT_VERSION     0x0800

#define CHECKED_DELETE(x)       if(x != NULL){ delete x; x = NULL; }
#define CHECKED_RELEASE(x)      if(x != NULL){ x->Release(); x = NULL; }
#define CHECKED_DELETE_ARRAY(x) if(x != NULL){ delete [] x; x = NULL; }

#define CoreInstance			CCore::GetSingletonPtr()
#define EngineInstance			CEngine::GetSingletonPtr()
#define GraphicsInstance		dynamic_cast<CGraphicsManager*>(CCore::GetSingletonPtr()->GetResource("graphics_manager"))
#define SoundInstance			CSoundManager::GetSingletonPtr()
#define EffectManagerInstance	dynamic_cast<CEffectManager*>(CCore::GetSingletonPtr()->GetResource("effect_manager"))
#define InputManagerInstance	dynamic_cast<CInputManager*>(CCore::GetSingletonPtr()->GetResource("input_manager"))
#define ActionManagerInstance	dynamic_cast<CActionManager*>(CCore::GetSingletonPtr()->GetResource("action_manager"))
#define EngineConfigInstance	CEngineConfig::GetSingletonPtr()
#define LanguageInstance		dynamic_cast<CLanguageManager*>(CCore::GetSingletonPtr()->GetResource("language_manager"))
#define FontInstance			dynamic_cast<CFontManager*>(CCore::GetSingletonPtr()->GetResource("font_manager"))
#define ROTMInstance			dynamic_cast<CRenderableObjectTechniqueManager*>(CCore::GetSingletonPtr()->GetResource("renderable_technique_manager"))
#define SMeshMInstance			dynamic_cast<CStaticMeshManager*>(CCore::GetSingletonPtr()->GetResource("static_mesh_manager"))
#define AnimatedMInstance		dynamic_cast<CAnimatedModelsManager*>(CCore::GetSingletonPtr()->GetResource("animated_manager"))
#define ROMLInstance			dynamic_cast<CRenderableObjectsLayersManager*>(CCore::GetSingletonPtr()->GetResource("renderable_manager_layer"))
#define SRCMInstance			dynamic_cast<CSceneRendererCommandManager*>(CCore::GetSingletonPtr()->GetResource("scene_render_command_manager"))
#define CameraMInstance			dynamic_cast<CCameraManager*>(CCore::GetSingletonPtr()->GetResource("camera_manager"))
#define ScriptMInstance			dynamic_cast<CScriptManager*>(CCore::GetSingletonPtr()->GetResource("script_manager"))
#define LightMInstance			dynamic_cast<CLightManager*>(CCore::GetSingletonPtr()->GetResource("light_manager"))
#define PhysXMInstance			dynamic_cast<CPhysicsManager*>(CCore::GetSingletonPtr()->GetResource("physics_manager"))
#define EnemyMInstance			dynamic_cast<CEnemyManager*>(CCore::GetSingletonPtr()->GetResource("enemy_manager"))
#define TriggersMInstance		dynamic_cast<CTriggerManager*>(CCore::GetSingletonPtr()->GetResource("trigger_manager"))
#define BillBoardInstance		dynamic_cast<CBillboard*>(CCore::GetSingletonPtr()->GetResource("billboard"))
#define ParticleMInstance		dynamic_cast<CParticleManager*>(CCore::GetSingletonPtr()->GetResource("particle_manager"))
#define GizmosMInstance			dynamic_cast<CGizmosManager*>(CCore::GetSingletonPtr()->GetResource("gizmos_manager"))
#define TextureMInstance		dynamic_cast<CTextureManager*>(CCore::GetSingletonPtr()->GetResource("texture_manager"))

#define LOG_ERROR_APPLICATION( x, ... ) CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR, x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  ) CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  ) CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, x, __VA_ARGS__ )

#ifdef _DEBUG

#define ASSERT(expr, msg) \
  {\
    static char s_text[199] = ""; \
    if ( !( expr ) ) \
    {\
      static int callIt = 1; \
      if ( callIt )\
      { \
        wsprintf( s_text, "Expression: %s\nMessage: ( %s )\nFile '%s' Line %d\nOk:Continue\nCancel:Do not show more asserts", #expr, #msg, __FILE__, __LINE__ ); \
        switch ( ::MessageBox( NULL, s_text, "ASSERTION ERROR", MB_ICONEXCLAMATION | MB_OKCANCEL ) ) \
        {\
          case IDCANCEL: \
          { \
            callIt = 0; \
            _asm { int 3 } \
            break;\
          } \
        } \
      } \
    } \
  }\

#else

#define ASSERT(expr, msg) assert( expr && msg );

#endif

#define FATAL_ERROR(msg) \
  {\
    static char s_text[199] = ""; \
    wsprintf( s_text, "Message: ( %s )\nFile '%s' Line %d", #msg, __FILE__, __LINE__ ); \
    switch ( ::MessageBox( NULL, s_text, "FATAL ERROR", MB_ICONERROR | MB_OK ) ) \
    {\
      case IDOK: \
      { \
        ::exit( EXIT_FAILURE ); \
        break;\
      } \
    } \
  }\

#endif