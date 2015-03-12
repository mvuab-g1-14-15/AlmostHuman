#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Utils\LogRender.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

CEngine::CEngine() :
  m_pCore( new CCore() ),
  m_pLogRender( new CLogRender() ),
  m_pProcess( 0 )
{
}

CEngine::~CEngine()
{
  CHECKED_DELETE( m_pCore );
  CHECKED_DELETE( m_pProcess );
  CHECKED_DELETE( m_pLogRender );
}

void CEngine::Update()
{
  // Update Time (EEEPC 1215B): 6.5ms
  m_pCore->Update();
  
  // Update Time (EEEPC 1215B): 5.0ms
  m_pProcess->Update();
 
  // Update Time (EEEPC 1215B): 0.4ms
  m_pLogRender->Update();
}

void CEngine::Render()
{
  /*CSceneRendererCommandManager* srcm = CCore::GetSingletonPtr()->GetSceneRendererCommandManager();
  srcm->Execute();
  return;*/

  CGraphicsManager* pGraphicsManager = GraphicsInstance ;
  pGraphicsManager->BeginRender();
  pGraphicsManager->SetupMatrices();

  // Render Time (EEEPC 1215B): 4.5ms
  m_pCore->Render();

  // Render Time (EEEPC 1215B): 15.5ms
  m_pProcess->Render();
    
  // Render Time (EEEPC 1215B): 1.0ms
  m_pLogRender->Render();
  
   // Render Time (EEEPC 1215B): 1.7ms
#if _DEBUG
  pGraphicsManager->DisableZBuffering();
  pGraphicsManager->EnableAlphaBlend();
  m_pProcess->RenderDebugInfo();
  pGraphicsManager->DisableAlphaBlend();
  pGraphicsManager->EnableZBuffering();
#endif
  pGraphicsManager->EndRender();
}

void CEngine::Init( CProcess* apProcess, const std::string& aConfigPath, HWND aWindowId )
{
  m_pCore->Init( aConfigPath, aWindowId );
  m_pLogRender->SetLinePerPage( 20 );
  m_pProcess = apProcess;
  m_pProcess->Init();
}
