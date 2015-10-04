#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"

#include "ActionManager.h"

#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "SceneRenderComands\RenderGUISceneRendererCommand.h"
#include "ScriptManager.h"

#include <iostream>
#include "Utils/StringUtils.h"

CEngine::CEngine()
  : m_pEngineManagers( new CEngineManagers( EngineConfigInstance->GetManagersPath() ) )
  , m_pProcess( 0 )
  , m_pTimer( new CTimer( 30 ) )
  , m_Play( false )
  , mConsoleEnabled( false )
{
}

CEngine::~CEngine()
{
  CHECKED_DELETE( m_pEngineManagers );
  CHECKED_DELETE( m_pProcess );
  CHECKED_DELETE( m_pTimer );
}

void CEngine::Execute()
{
    ProcessInputs();
    Update();
    Render();
}

void CEngine::Update()
{
  m_pTimer->Update();
  m_pEngineManagers->Update();
  m_pProcess->ProcessReloads();
  m_pProcess->ProcessKey();
  m_pProcess->Update();
}

void CEngine::Render()
{
  //if ( m_Play )
    if(!m_pEngineManagers->GetGraphicsManager()->isDeviceLost()) 
        SRCMInstance->Execute();
  /*  else
      {
      CRenderGUISceneRendererCommand* l_GUICommand = SRCMInstance->GetCommandGUI();
      l_GUICommand->Execute( *GraphicsInstance );
      }*/
}

void CEngine::ProcessInputs()
{
  ActionManagerInstance->ProcessInputs();
}

void CEngine::SetRunnigProcess( CProcess* aProcess )
{
  m_pProcess = aProcess;
}


void CEngine::Init( CEngineConfig* aEngineConfig )
{
  m_RenderTime = m_RenderTarget = 1.0f / 30.0f;

  if ( aEngineConfig )
  {
    mConsoleEnabled = CEngineConfig::GetSingletonPtr()->GetTraceOutputMode() == CEngineConfig::eConsole;
    // Init the managers of the engine
    m_pEngineManagers->Init();

    // Init the videogame
    m_pProcess->Init();
  }
}

void CEngine::Trace( const std::string& TraceStr )
{
  if ( mConsoleEnabled )
  {
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
    std::cout << TraceStr << std::endl << std::endl;
  }
  else
  {
    std::stringstream lMsg;
    lMsg << "[APPLICATION]" << TraceStr << std::endl;
    OutputDebugString( lMsg.str().c_str() );
  }

}

void CEngine::QuitGame()
{
  PostQuitMessage( 0 );
}
