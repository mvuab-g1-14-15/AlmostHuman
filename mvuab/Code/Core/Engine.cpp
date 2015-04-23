#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"

#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include <iostream>

CEngine::CEngine()
    : m_pEngineManagers( new CEngineManagers( EngineConfigInstance->GetManagersPath() ) )
    , m_pProcess( 0 )
    , m_pTimer( new CTimer( 30 ) )
{
}

CEngine::~CEngine()
{
    CHECKED_DELETE( m_pEngineManagers );
    CHECKED_DELETE( m_pProcess );
    CHECKED_DELETE( m_pTimer );
}

void CEngine::Update()
{
    m_pEngineManagers->Update();
    m_pProcess->Update();
    m_pTimer->Update();
}

void CEngine::Render()
{
    SRCMInstance->Execute();
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
    m_RenderTime = m_RenderTarget = 1.0f / 30.0f;

    if ( aEngineConfig )
    {
        // Init the managers of the engine
        m_pEngineManagers->Init();

        // Init the videogame
        m_pProcess->Init();
    }
}

void CEngine::Trace( const std::string& TraceStr )
{
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hConsole,
                             FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
    std::cout << TraceStr << std::endl << std::endl;
}
