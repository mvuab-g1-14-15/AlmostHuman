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
  m_pCore->Update();
  m_pProcess->Update();
  m_pLogRender->Update();
}

void CEngine::Render()
{
  CSceneRendererCommandManager* srcm = CCore::GetSingletonPtr()->GetSceneRendererCommandManager();
  srcm->Execute();
  return;
  //// Obtain an instance to the graphics manager
  CGraphicsManager* pGraphicsManager = GraphicsInstance ;
  //pGraphicsManager->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  pGraphicsManager->BeginRender();
  pGraphicsManager->SetupMatrices();
  m_pCore->Render();
  m_pProcess->Render();
  m_pLogRender->Render();
  //pGraphicsManager->RenderCursor();
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