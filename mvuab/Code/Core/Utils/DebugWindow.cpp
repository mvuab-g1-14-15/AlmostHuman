#include "DebugWindow.h"
#include "Assert.h"
#include <windows.h>
#include <vector>
#include "Utils\BaseUtils.h"
#include "ActionManager.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "Logger\Logger.h"
//#include "Utils\MemLeaks.h"

CDebugWindow::CDebugWindow(Vect2i aWindowPosition) :
		m_bIsVisible(true),
		m_WindowPosition(aWindowPosition),
        m_FontColor_debug(colBLACK),
	    m_FontColor_release(colWHITE)
{

}

CDebugWindow::~CDebugWindow()
{

}

void CDebugWindow::ToggleVisibility()
{
	SetVisible(!m_bIsVisible);
}

void CDebugWindow::Render()
{
	CGraphicsManager* renderManager = CGraphicsManager::GetSingletonPtr();
	CFontManager * fm = CFontManager::GetSingletonPtr();
	if( m_bIsVisible )
		RenderInfo();
}