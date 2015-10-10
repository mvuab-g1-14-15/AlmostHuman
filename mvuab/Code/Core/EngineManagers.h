#ifndef ENGINE_MANAGER_H
#define ENGINE_MANAGER_H
#pragma once

#include "Utils/Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "XML/XMLTreeNode.h"
#include "Utils\SingletonPattern.h"
#include "Utils\Manager.h"

#include <vector>
#include <string>

//Forward declaration
class CGraphicsManager;
class CEffectManager;
class CInputManager;
class CActionManager;
class CLanguageManager;
class CFontManager;
class CRenderableObjectTechniqueManager;
class CStaticMeshManager;
class CAnimatedModelsManager;
class CScene;
class CSceneRendererCommandManager;
class CCameraManager;
class CScriptManager;
class CLightManager;
class CPhysicsManager;
class CEnemyManager;
class CTriggerManager;
class CGizmosManager;
class CTextureManager;
class CWWSoundManager;
class CGUIManager;
class CBillboardManager;
class CCountDownTimerManager;
class CCinematicManager;
class CConsole;
class CLensFlareManager;
class CIdManager;
class CEditorsManager;
class CParticleSystemManager;

class CEngineManagers : public CTemplatedVectorMapManager< CManager >, public CSingleton< CEngineManagers >
{
public:
  CEngineManagers( const std::string& aPath );
  virtual ~CEngineManagers();
  virtual void Init();
  virtual void Update();
  virtual void Render();

  void Reload();
  void Release();

  CGraphicsManager*                 GetGraphicsManager()      const;
  CEffectManager*                   GetEffectsManager()       const;
  CInputManager*                    GetInputManager() const;
  CActionManager*                   GetActionManager() const;
  CLanguageManager*                 GetLanguageManager() const;
  CFontManager*                     GetFontManager() const;
  CScene*               GetSceneManager() const;
  CStaticMeshManager*               GetStaticMeshManager() const;
  CRenderableObjectTechniqueManager* GetROTManager() const;
  CAnimatedModelsManager*           GetAnimatedModelsManager() const;
  CSceneRendererCommandManager*     GetSceneRendererCmdManager() const;
  CCameraManager*                   GetCameraManager() const;
  CScriptManager*                   GetScriptManager() const;
  CLightManager*                    GetLightManager() const;
  CPhysicsManager*                  GetPhysicsManager() const;
  CEnemyManager*                    GetEnemyManager() const;
  CTriggerManager*                  GetTriggerManager() const;
  CGizmosManager*                   GetGizmosManager() const;
  CTextureManager*                  GetTextureManager() const;
  CWWSoundManager*                  GetSoundManager() const;
  CGUIManager*                      GetGUIManager() const;
  CBillboardManager*                GetBillboardManager() const;
  CCountDownTimerManager*           GetCountDownTimerManager() const;
  CCinematicManager*                GetCinematicManager() const;
  CLensFlareManager*                GetLensFlareManager() const;
  CIdManager*                       GetIdManager() const;
  CEditorsManager*                  GetEditorsManager() const;
  CParticleSystemManager*           GetParticleSystemManager() const;

  GET_SET_REF( std::string, ManagersPath );


private:
  CEditorsManager*                         m_pEditorsManager;
  CGraphicsManager*                      m_pGraphicsManager;
  CEffectManager*                        m_pEffectManager;
  CInputManager*                         m_pInputManager;
  CActionManager*                        m_pActionManager;
  CLanguageManager*                      m_pLanguageManager;
  CFontManager*                          m_pFontManager;
  CRenderableObjectTechniqueManager*     m_pRenderableObjectTechniqueManager;
  CStaticMeshManager*                    m_pStaticMeshManager;
  CAnimatedModelsManager*                m_pAnimatedModelsManager;
  CScene*                  m_pScene;
  CSceneRendererCommandManager*          m_pSceneRendererCommandManager;
  CCameraManager*                        m_pCameraManager;
  CScriptManager*                        m_pScriptManager;
  CLightManager*                         m_pLightManager;
  CPhysicsManager*                       m_pPhysicsManager;
  CEnemyManager*                         m_pEnemyManager;
  CTriggerManager*                       m_pTriggerManager;
  CGizmosManager*                        m_pGizmosManager;
  CTextureManager*                       m_pTextureManager;
  CWWSoundManager*                       m_pSoundManager;
  CGUIManager*                           m_pGUIManager;
  CBillboardManager*                     m_BillboardManager;
  CCountDownTimerManager*                m_pCountDownTimerManager;
  CCinematicManager*                     m_pCinematicManager;
  CLensFlareManager*                     m_pLensFlareManager;
  CIdManager*                            m_pIdManager;
  CConsole*                              m_pConsole;
  CParticleSystemManager*                m_pParticleSystemManager;
  std::string                            m_ManagersPath;
};

#endif // ENGINE_MANAGER_H