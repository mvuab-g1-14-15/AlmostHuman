#include "Engine.h"
#include "Utils\Defines.h"
#include "Process.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Utils\LogRender.h"

CEngine::CEngine() :
	m_pCore( new CCore() ),
	m_pLogRender( new CLogRender()),
	m_pProcess( 0 ),
	m_timer(30)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE( m_pCore );
	CHECKED_DELETE( m_pProcess );
	CHECKED_DELETE( m_pLogRender );
}

void CEngine::Update(float32 deltaTime)
{
	m_timer.Update();
	m_pCore->Update(m_timer.GetElapsedTime());
	m_pProcess->Update(m_timer.GetElapsedTime());
	m_pLogRender->Update(m_timer.GetElapsedTime());
}

void CEngine::Render()
{	
	// Obtain an instance to the graphics manager
	CGraphicsManager* pGraphicsManager = GraphicsInstance;
	pGraphicsManager->BeginRender();
	pGraphicsManager->SetupMatrices( m_pProcess->GetCamera());
	m_pCore->Render();
	m_pProcess->Render();
	m_pLogRender->Render();

#if _DEBUG
	pGraphicsManager->DisbaleZBuffering();
	pGraphicsManager->EnableAlphaBlend();
	m_pProcess->RenderDebugInfo();
	pGraphicsManager->DisbaleAlphaBlend();
	pGraphicsManager->EnableZBuffering();
#endif

	pGraphicsManager->EndRender();
}

void CEngine::Init( CProcess * apProcess, const std::string &aConfigPath, HWND aWindowId )
{
	m_pCore->Init(aConfigPath, aWindowId);
	m_pProcess = apProcess;
	m_pProcess->Init();
	m_pLogRender->SetLinePerPage(20);
}