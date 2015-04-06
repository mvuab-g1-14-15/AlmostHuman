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
#include "EngineConfig.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

CEngine::CEngine()
  : m_pCore( new CCore() )
  , m_pLogRender( new CLogRender() )
  , m_pProcess( 0 )
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
  m_pCore->GetSceneRendererCommandManager()->Execute();
}

void CEngine::ProcessInputs()
{
  // TODO: Decirle al action manager que procese los inputs
}

void CEngine::SetRunnigProcess( CProcess* aProcess )
{
  m_pProcess = aProcess;
}

void CEngine::Init( CEngineConfig* aEngineConfig )
{
  if ( aEngineConfig )
  {
    // Init the core of the engine
    m_pCore->Init( aEngineConfig->GetWindowId() );
    // Init the videogame
    m_pProcess->Init();
  }
}
