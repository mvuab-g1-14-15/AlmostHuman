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

#include "Brofiler.h"

CEngine::CEngine()
    : m_pEngineManagers(new CEngineManagers(EngineConfigInstance->GetManagersPath()))
    , m_pProcess(0)
    , m_pTimer(new CTimer(EngineConfigInstance->GetFps()))
    , m_Play(false)
    , mConsoleEnabled(false)
    , mLastTraceMsg("")
    , m_AccTime(0.0f)
{
    m_RenderTime = 0.0f;
    m_RenderTarget = m_pTimer->GetConstFrameTime();
}

CEngine::~CEngine()
{
    CHECKED_DELETE(m_pEngineManagers);
    CHECKED_DELETE(m_pProcess);
    CHECKED_DELETE(m_pTimer);
}

void CEngine::Execute()
{
    ProcessInputs();
    Update();
    Render();
}

void CEngine::Update()
{
    BROFILER_CATEGORY("CEngine:Update", Profiler::Color::LightBlue)
    m_pTimer->Update();

    m_pProcess->ProcessReloads();
    m_RenderTime = m_pTimer->GetElapsedTime();

    m_AccTime += m_RenderTime;
    while(m_AccTime >= m_RenderTarget)
    {
        m_AccTime -= m_RenderTarget;
        m_pProcess->ProcessKey();

        m_pEngineManagers->Update();
        m_pProcess->Update();
    }
}

void CEngine::Render()
{
    BROFILER_CATEGORY("CEngine:Render", Profiler::Color::Red)
    if(!m_pEngineManagers->GetGraphicsManager()->isDeviceLost())
        SRCMInstance->Execute();
}

void CEngine::ProcessInputs()
{
    ActionManagerInstance->ProcessInputs();
}

void CEngine::SetRunnigProcess(CProcess* aProcess)
{
    m_pProcess = aProcess;
}


void CEngine::Init(CEngineConfig* aEngineConfig)
{
    if(aEngineConfig)
    {
        mConsoleEnabled = CEngineConfig::GetSingletonPtr()->GetTraceOutputMode() == CEngineConfig::eConsole;
        // Init the managers of the engine
        m_pEngineManagers->Init();

        // Init the videogame
        m_pProcess->Init();
    }
}


#ifdef NO_TRACE
void CEngine::TraceOnce(const std::string& /*TraceStr*/) {}
void CEngine::Trace(const std::string& /*TraceStr*/) {}
#else
void CEngine::TraceOnce(const std::string& TraceStr)
{
    if(TraceStr == mLastTraceMsg)
        return;

    Trace(TraceStr);

    mLastTraceMsg = TraceStr;
}
void CEngine::Trace(const std::string& TraceStr)
{
    if(mConsoleEnabled)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << TraceStr << std::endl << std::endl;
    }
    else
    {
        std::stringstream lMsg;
        lMsg << "[APPLICATION]" << TraceStr << std::endl;
        OutputDebugString(lMsg.str().c_str());
    }
}
#endif

void CEngine::QuitGame()
{
    PostQuitMessage(0);
}
