#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Utils\LogRender.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"
#include "EngineConfig.h"

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
  CHECKED_DELETE( m_pEngineConfig );
}

void CEngine::Update()
{
  m_pCore->Update();
  m_pProcess->Update();
  m_pLogRender->Update();
}

void CEngine::Render()
{
    m_pCore->GetSceneRendererCommandManager()->Execute();
}

void CEngine::Init( CProcess* apProcess, const std::string& aConfigPath, HWND aWindowId )
{
  m_pEngineConfig = new CEngineConfig(aConfigPath);
  m_pCore->Init( aConfigPath, aWindowId );
  m_pLogRender->SetLinePerPage( 20 );
  m_pProcess = apProcess;
  m_pProcess->Init();
}
