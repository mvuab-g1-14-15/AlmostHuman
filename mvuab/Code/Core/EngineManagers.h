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
class CRenderableObjectsLayersManager;
class CSceneRendererCommandManager;    
class CCameraManager;                  
class CScriptManager;                  
class CLightManager;                 
class CPhysicsManager;                 
class CEnemyManager;                  
class CTriggerManager;                 
class CBillboard;                     
class CParticleManager;                
class CGizmosManager;                  
class CTextureManager;                 

class CEngineManagers : public CTemplatedVectorMapManager< CManager >, public CSingleton< CEngineManagers >
{
public:
  CEngineManagers( const std::string & aPath );
  virtual ~CEngineManagers();
  virtual void Init();
  virtual void Update();
  virtual void Render();

  void Release();

  GET_SET_PTR( CGraphicsManager, GraphicsManager)
  GET_SET_PTR( CEffectManager, EffectManager)
  GET_SET_PTR( CInputManager, InputManager)
  GET_SET_PTR( CActionManager, ActionManager)
  GET_SET_PTR( CLanguageManager, LanguageManager)
  GET_SET_PTR( CFontManager, FontManager)
  GET_SET_PTR( CRenderableObjectTechniqueManager, RenderableObjectTechniqueManager)
  GET_SET_PTR( CStaticMeshManager, StaticMeshManager)
  GET_SET_PTR( CAnimatedModelsManager, AnimatedModelsManager)
  GET_SET_PTR( CRenderableObjectsLayersManager, RenderableObjectsLayersManager)
  GET_SET_PTR( CSceneRendererCommandManager, SceneRendererCommandManager)
  GET_SET_PTR( CCameraManager, CameraManager)
  GET_SET_PTR( CScriptManager, ScriptManager)
  GET_SET_PTR( CLightManager, LightManager)
  GET_SET_PTR( CPhysicsManager, PhysicsManager)
  GET_SET_PTR( CEnemyManager, EnemyManager)
  GET_SET_PTR( CTriggerManager, TriggerManager)
  GET_SET_PTR( CBillboard , Billboard )
  GET_SET_PTR( CParticleManager, ParticleManager)
  GET_SET_PTR( CGizmosManager, GizmosManager)
  GET_SET_PTR( CTextureManager, TextureManager)

private:
  CGraphicsManager*                     m_pGraphicsManager;
  CEffectManager*                       m_pEffectManager;
  CInputManager*                        m_pInputManager;
  CActionManager*                       m_pActionManager;                  
  CLanguageManager*                     m_pLanguageManager;                 
  CFontManager*                         m_pFontManager;              
  CRenderableObjectTechniqueManager*    m_pRenderableObjectTechniqueManager;
  CStaticMeshManager*                   m_pStaticMeshManager;               
  CAnimatedModelsManager*               m_pAnimatedModelsManager;         
  CRenderableObjectsLayersManager*      m_pRenderableObjectsLayersManager;  
  CSceneRendererCommandManager*         m_pSceneRendererCommandManager;      
  CCameraManager*                       m_pCameraManager;                 
  CScriptManager*                       m_pScriptManager;                  
  CLightManager*                        m_pLightManager;                    
  CPhysicsManager*                      m_pPhysicsManager;                  
  CEnemyManager*                        m_pEnemyManager;                    
  CTriggerManager*                      m_pTriggerManager;                  
  CBillboard*                           m_pBillboard;                       
  CParticleManager*                     m_pParticleManager;                 
  CGizmosManager*                       m_pGizmosManager;                   
  CTextureManager*                      m_pTextureManager;

  std::string                           m_ManagersPath;
};

#endif // ENGINE_MANAGER_H