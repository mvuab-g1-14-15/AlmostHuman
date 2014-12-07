#include "FPSDebugWindow.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
//#include "Utils\MemLeaks.h"

CFPSDebugWindow::CFPSDebugWindow(Vect2i aWindowPosition) :
	CDebugWindow(aWindowPosition),
	m_NumberFPSCount(0),
	m_NumberFPSSum(0),
	m_FPS(0),
	m_MinFps(1000),
	m_MaxFps(0)
{
	m_WindowHeight = 85;
	m_WindowWidth = 100;
}

CFPSDebugWindow::~CFPSDebugWindow()
{

}

void CFPSDebugWindow::Update (float32 deltaTime)
{
	++m_NumberFPSCount;
	m_FPS = (uint32)(1.0f/deltaTime);
	m_NumberFPSSum += m_FPS;
	if( m_NumberFPSCount > 50 ) // Esperamos a que se estabilicen
	{
		m_MinFps = ( m_MinFps < m_FPS ) ? m_MinFps : m_FPS;
		m_MaxFps = ( m_MaxFps > m_FPS ) ? m_MaxFps : m_FPS;
	}
}

void CFPSDebugWindow::RenderInfo()
{
	CFontManager* fm = CFontManager::GetSingletonPtr();
	CGraphicsManager* renderManager = CGraphicsManager::GetSingletonPtr();

#ifdef _DEBUG
    const CColor &l_FontColor = m_FontColor_debug;
#else
    const CColor &l_FontColor = m_FontColor_release;
#endif

	uint32 w, h;
	renderManager->GetWidthAndHeight(w,h);
	m_WindowPosition.y = uint32(h - (85.0f)); //85 = 5 lines *17 height per line
	uint32 incY = fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y,  l_FontColor, "FPS");
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Current: %d", m_FPS );
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Average: %d", (m_NumberFPSCount) ? (uint32)m_NumberFPSSum / m_NumberFPSCount : 0 );
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Min: %d", m_MinFps );
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Max: %d", m_MaxFps );
}
