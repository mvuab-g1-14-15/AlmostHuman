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

#define CHECKED_DELETE(x)       if(x != NULL){ delete x; x = NULL; }
#define CHECKED_RELEASE(x)      if(x != NULL){ x->Release(); x = NULL; }
#define CHECKED_DELETE_ARRAY(x) if(x != NULL){ delete [] x; x = NULL; }

#define EngineInstance          CEngine::GetSingletonPtr()
#define EngineManagerInstance   CEngineManagers::GetSingletonPtr()
#define GraphicsInstance        CEngineManagers::GetSingletonPtr()->GetGraphicsManager()
#define SoundInstance           CSoundManager::GetSingletonPtr()
#define EffectManagerInstance   CEngineManagers::GetSingletonPtr()->GetEffectsManager()
#define InputManagerInstance    CEngineManagers::GetSingletonPtr()->GetInputManager()
#define ActionManagerInstance   CEngineManagers::GetSingletonPtr()->GetActionManager()
#define EngineConfigInstance    CEngineConfig::GetSingletonPtr()
#define LanguageInstance        CEngineManagers::GetSingletonPtr()->GetLanguageManager()
#define FontInstance            CEngineManagers::GetSingletonPtr()->GetFontManager()
#define ROTMInstance            CEngineManagers::GetSingletonPtr()->GetROTManager()
#define SMeshMInstance          CEngineManagers::GetSingletonPtr()->GetStaticMeshManager()
#define AnimatedMInstance       CEngineManagers::GetSingletonPtr()->GetAnimatedModelsManager()
#define ROLMInstance            CEngineManagers::GetSingletonPtr()->GetROLManager()
#define SRCMInstance            CEngineManagers::GetSingletonPtr()->GetSceneRendererCmdManager()
#define CameraMInstance         CEngineManagers::GetSingletonPtr()->GetCameraManager()
#define ScriptMInstance         CEngineManagers::GetSingletonPtr()->GetScriptManager()
#define LightMInstance          CEngineManagers::GetSingletonPtr()->GetLightManager()
#define PhysXMInstance          CEngineManagers::GetSingletonPtr()->GetPhysicsManager()
#define EnemyMInstance          CEngineManagers::GetSingletonPtr()->GetEnemyManager()
#define TriggersMInstance       CEngineManagers::GetSingletonPtr()->GetTriggerManager()
#define PSMan                   CEngineManagers::GetSingletonPtr()->GetParticleManager()
#define GizmosMInstance         CEngineManagers::GetSingletonPtr()->GetGizmosManager()
#define TextureMInstance        CEngineManagers::GetSingletonPtr()->GetTextureManager()
#define SoundMan                CEngineManagers::GetSingletonPtr()->GetSoundManager()
#define GUIInstance             CEngineManagers::GetSingletonPtr()->GetGUIManager()
#define BillboardMan            CEngineManagers::GetSingletonPtr()->GetBillboardManager()
#define TimerInstance           CEngine::GetSingletonPtr()->GetTimer()
#define CountDownTimerInstance            CEngineManagers::GetSingletonPtr()->GetCountDownTimerManager();

#ifdef _DEBUG

#define ASSERT(expr, msg, ... ) \
    {\
        if ( !( expr ) ) \
        {\
            CMessageHandler::Assert( __FILE__, __LINE__, msg, __VA_ARGS__ );\
        } \
    }\

#define LOG_ERROR_APPLICATION( x, ... )    CLogger::GetSingletonPtr()->AddNewLog( eLogError,   typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  ) CLogger::GetSingletonPtr()->AddNewLog( eLogWarning, typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  )    CLogger::GetSingletonPtr()->AddNewLog( eLogInfo,    typeid(this).name(), __FILE__, __LINE__, x, __VA_ARGS__ )

#else

#define ASSERT(expr, msg, ... ) do{ } while(0); // The compiler will delete this line in release
#define LOG_ERROR_APPLICATION( x, ... ) do{ } while(0); // The compiler will delete this line in release
#define LOG_WARNING_APPLICATION( x, ...  ) do{ } while(0); // The compiler will delete this line in release
#define LOG_INFO_APPLICATION( x, ...  ) do{ } while(0); // The compiler will delete this line in release

#endif

#define FATAL_ERROR( msg, ... ) \
    {\
        CMessageHandler::FatalError( __FILE__, __LINE__, msg, __VA_ARGS__ );\
    }\

#endif // _DEFINES_H_