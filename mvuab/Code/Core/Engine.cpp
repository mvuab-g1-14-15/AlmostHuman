#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Utils\LogRender.h"
#include "Timer\Timer.h"

CEngine::CEngine() :
    m_pCore( new CCore() ),
    m_pLogRender( new CLogRender()),
    m_pProcess( 0 ),
    m_timer(new CTimer(30))
{
}

CEngine::~CEngine()
{
    CHECKED_DELETE( m_pCore );
    CHECKED_DELETE( m_pProcess );
    CHECKED_DELETE( m_pLogRender );
    CHECKED_DELETE( m_timer );
}

void CEngine::Update()
{
    m_timer->Update();
    m_pCore->Update(m_timer->GetElapsedTime());
    m_pProcess->Update(m_timer->GetElapsedTime());
    m_pLogRender->Update(m_timer->GetElapsedTime());
}

void CEngine::Render()
{    
    // Obtain an instance to the graphics manager
    CGraphicsManager* pGraphicsManager = GraphicsInstance;
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

void CEngine::Init( CProcess * apProcess, const std::string &aConfigPath, HWND aWindowId )
{
    m_pCore->Init(aConfigPath, aWindowId);
    m_pLogRender->SetLinePerPage(20);

    m_pProcess = apProcess;
    m_pProcess->Init();
    
}