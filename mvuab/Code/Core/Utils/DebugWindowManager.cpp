#include "DebugWindowManager.h"
#include "FPSDebugWindow.h"
#include "CameraDebugWindow.h"
#include "ActionManager.h"
#include "Math\Vector2.h"
#include "Utils\Defines.h"

CDebugWindowManager::CDebugWindowManager() :
        m_bIsVisible(true),
        m_CurrentDebugWindow(0)
{
    CFPSDebugWindow* p_FPSDebugWindow = new CFPSDebugWindow(Math::Vect2i( 5, 0 ));
    m_vDebugWindows.push_back(p_FPSDebugWindow);
    CCameraDebugWindow* p_CameraDebugWindow = new CCameraDebugWindow(Math::Vect2i( p_FPSDebugWindow->GetWindowWidth(), 0 ));
    m_vDebugWindows.push_back(p_CameraDebugWindow);
}

CDebugWindowManager::~CDebugWindowManager()
{
    std::vector<CDebugWindow*>::iterator itb = m_vDebugWindows.begin(), ite = m_vDebugWindows.end();
    for( ; ite != itb; ++itb )
    {
        CDebugWindow* &p_DWindow = *itb;
        CHECKED_DELETE(p_DWindow);
    }

    m_vDebugWindows.clear();
}

void CDebugWindowManager::ToggleVisibility()
{
    SetVisible(!m_bIsVisible);
}

void CDebugWindowManager::Render()
{
    for( uint32 i = 0; i < m_vDebugWindows.size(); ++i )
        m_vDebugWindows[i]->Render();
}

void CDebugWindowManager::Update()
{
    for( uint32 i = 0; i < m_vDebugWindows.size(); ++i )
        m_vDebugWindows[i]->Update();
}