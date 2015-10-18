#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "Types.h"
#include <assert.h>
#include "Logger\Logger.h"
#include "MessageHandler\MessageHandler.h"
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

#define CHECKED_DELETE(x)       { if(x != NULL){ delete x; x = NULL; } }
#define CHECKED_RELEASE(x)      { if(x != NULL){ x->Release(); x = NULL; } }
#define CHECKED_DELETE_ARRAY(x) { if(x != NULL){ delete [] x; x = NULL; } }

#define EngineInstance          CEngine::GetSingletonPtr()
#define EngineManagerInstance   CEngineManagers::GetSingletonPtr()
#define GraphicsInstance        CEngineManagers::GetSingletonPtr()->GetGraphicsManager()
#define SoundInstance           CEngineManagers::GetSingletonPtr()->GetSoundManager()
#define EffectManagerInstance   CEngineManagers::GetSingletonPtr()->GetEffectsManager()
#define InputManagerInstance    CEngineManagers::GetSingletonPtr()->GetInputManager()
#define ActionManagerInstance   CEngineManagers::GetSingletonPtr()->GetActionManager()
#define EngineConfigInstance    CEngineConfig::GetSingletonPtr()
#define LanguageInstance        CEngineManagers::GetSingletonPtr()->GetLanguageManager()
#define FontInstance            CEngineManagers::GetSingletonPtr()->GetFontManager()
#define ROTMInstance            CEngineManagers::GetSingletonPtr()->GetROTManager()
#define SMeshMInstance          CEngineManagers::GetSingletonPtr()->GetStaticMeshManager()
#define AnimatedMInstance       CEngineManagers::GetSingletonPtr()->GetAnimatedModelsManager()
#define SceneInstance           CEngineManagers::GetSingletonPtr()->GetSceneManager()
#define SRCMInstance            CEngineManagers::GetSingletonPtr()->GetSceneRendererCmdManager()
#define CameraMInstance         CEngineManagers::GetSingletonPtr()->GetCameraManager()
#define ScriptMInstance         CEngineManagers::GetSingletonPtr()->GetScriptManager()
#define LightMInstance          CEngineManagers::GetSingletonPtr()->GetLightManager()
#define PhysXMInstance          CEngineManagers::GetSingletonPtr()->GetPhysicsManager()
#define TriggersMInstance       CEngineManagers::GetSingletonPtr()->GetTriggerManager()
#define GizmosMInstance         CEngineManagers::GetSingletonPtr()->GetGizmosManager()
#define TextureMInstance        CEngineManagers::GetSingletonPtr()->GetTextureManager()
#define SoundMan                CEngineManagers::GetSingletonPtr()->GetSoundManager()
#define GUIInstance             CEngineManagers::GetSingletonPtr()->GetGUIManager()
#define BillboardMan            CEngineManagers::GetSingletonPtr()->GetBillboardManager()
#define TimerInstance           CEngine::GetSingletonPtr()->GetTimer()
#define CountDownTimerInstance  CEngineManagers::GetSingletonPtr()->GetCountDownTimerManager()
#define CinematicMInstance      CEngineManagers::GetSingletonPtr()->GetCinematicManager()
#define FlareMan                CEngineManagers::GetSingletonPtr()->GetLensFlareManager()
#define IdManager               CEngineManagers::GetSingletonPtr()->GetIdManager()
#define PSManager               CEngineManagers::GetSingletonPtr()->GetParticleSystemManager()

#define _USING_MESH_FOR_PHYSX

//#define FINAL_MODE

#ifdef FINAL_MODE

 #define ASSERT(expr, msg, ... ){}
 #define LOG_ERROR_APPLICATION( x, ... ){}
 #define LOG_WARNING_APPLICATION( x, ...  ){}
 #define LOG_INFO_APPLICATION( x, ...  ){}
 #define STATIC_LOG_INFO_APPLICATION( x, ...  ){}
 #define STATIC_LOG_ERROR_APPLICATION( x, ...  ){}
 #define STATIC_LOG_WARNING_APPLICATION( x, ...  ){}

#else // MODE_RELEASE, MODE_DEBUG

#define ASSERT(expr, msg, ... ) \
    {\
        static bool sIgnoreAssert = false; \
        if( ! (expr) )\
          CLogger::GetSingletonPtr()->AddNewLog( eLogError,   "assert", __FILE__, __LINE__, msg, __VA_ARGS__ );\
        if ( !sIgnoreAssert && !( (expr) ) ) \
        {\
            CMessageHandler::Assert( sIgnoreAssert, __FILE__, __LINE__, msg, __VA_ARGS__ );\
        } \
    }\

#define LOG_ERROR_APPLICATION( x, ... )                    CLogger::GetSingletonPtr()->AddNewLog( eLogError,   typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  )                CLogger::GetSingletonPtr()->AddNewLog( eLogWarning, typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  )                    CLogger::GetSingletonPtr()->AddNewLog( eLogInfo,    typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )
#define STATIC_LOG_INFO_APPLICATION( x, ...  )            CLogger::GetSingletonPtr()->AddNewLog( eLogInfo,    "static_method", __FILE__, __LINE__, x, __VA_ARGS__ )
#define STATIC_LOG_ERROR_APPLICATION( x, ...  )         CLogger::GetSingletonPtr()->AddNewLog( eLogWarning, "static_method", __FILE__, __LINE__, x, __VA_ARGS__ )
#define STATIC_LOG_WARNING_APPLICATION( x, ...  )       CLogger::GetSingletonPtr()->AddNewLog( eLogError,   "static_method", __FILE__, __LINE__, x, __VA_ARGS__ )

#endif

#define FATAL_ERROR( msg, ... ) \
    {\
        CMessageHandler::FatalError( __FILE__, __LINE__, msg, __VA_ARGS__ );\
    }\

#endif // _DEFINES_H_