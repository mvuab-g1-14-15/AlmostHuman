#include "FPSDebugWindow.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "Core.h"
#include "Timer\Timer.h"

CFPSDebugWindow::CFPSDebugWindow(Math::Vect2i aWindowPosition) :
    CDebugWindow(aWindowPosition)
{
    m_WindowHeight = 85;
    m_WindowWidth = 100;
}

CFPSDebugWindow::~CFPSDebugWindow()
{

}

void CFPSDebugWindow::Update()
{
}

void CFPSDebugWindow::RenderInfo()
{
    CFontManager* fm = CFontManager::GetSingletonPtr();
    CGraphicsManager* renderManager = CGraphicsManager::GetSingletonPtr();

#ifdef _DEBUG
    const Math::CColor &l_FontColor = m_FontColor_debug;
#else
    const Math::CColor &l_FontColor = m_FontColor_release;
#endif

    uint32 w, h;
    renderManager->GetWidthAndHeight(w,h);
    m_WindowPosition.y = uint32(h - (85.0f)); //85 = 5 lines *17 height per line
    uint32 incY = fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y,  l_FontColor, "FPS");
    incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Current: %d", uint32(FPS) );
}
