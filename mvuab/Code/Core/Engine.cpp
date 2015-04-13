#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"
#include "EngineConfig.h"
#include "EngineManagers.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

CEngine::CEngine()
    : m_pCore( new CCore() )    
    , m_pEngineManagers( new CEngineManagers( EngineConfigInstance->GetManagersPath() ) )
    , m_pProcess( 0 )
{
}

CEngine::~CEngine()
{
    CHECKED_DELETE( m_pCore );
    CHECKED_DELETE( m_pEngineManagers );
    CHECKED_DELETE( m_pProcess );
}

void CEngine::Update()
{
    m_pCore->Update();
    m_pEngineManagers->Update();
    m_pProcess->Update();
}

void CEngine::Render()
{
  SRCMInstance->Execute();
    //m_RenderNow -= m_pCore->GetTimer()->GetElapsedTime();
    //if(m_RenderNow > 0.0f) return;

    //m_pCore->GetSceneRendererCommandManager()->Execute();
    //m_RenderNow = m_RenderTime;
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

        // Init the core of the engine
        m_pCore->Init();

        // Init the managers of the engine
        m_pEngineManagers->Init();

        // Init the videogame
        m_pProcess->Init();

        // Set render time
        m_RenderTime = m_RenderTarget = 1.0f / aEngineConfig->GetMaxFps();
    }
}
