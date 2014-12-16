#include "CameraDebugWindow.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "Cameras\Camera.h"
//#include "Utils\MemLeaks.h"

CCameraDebugWindow::CCameraDebugWindow(Vect2i aWindowPosition) :
    CDebugWindow(aWindowPosition)
{
    m_WindowHeight = 85;
    m_WindowWidth = 100;
}

CCameraDebugWindow::~CCameraDebugWindow()
{

}

void CCameraDebugWindow::Update (float32 deltaTime)
{
}

void CCameraDebugWindow::RenderInfo()
{
    CFontManager* fm = CFontManager::GetSingletonPtr();
    CGraphicsManager* renderManager = CGraphicsManager::GetSingletonPtr();
    CCamera* pCamera = renderManager->GetCurrentCamera();
    
    if(!pCamera)
        return;
    
    uint32 w, h;
    renderManager->GetWidthAndHeight(w,h);
    m_WindowPosition.y = uint32(h - (85.0f)); //85 = 5 lines *17 height per line

    
#ifdef _DEBUG
    const CColor &l_FontColor = m_FontColor_debug;
#else
    const CColor &l_FontColor = m_FontColor_release;
#endif

    uint32 incY = fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y,  l_FontColor, "CAMERA");
    const Vect3f l_CameraPosition( pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z );
    incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Position: %d,%d,%d", (uint32)l_CameraPosition.x, (uint32)l_CameraPosition.y, (uint32)l_CameraPosition.z );
    const Vect3f l_CameraLookAt( pCamera->GetLookAt().x, pCamera->GetLookAt().y, pCamera->GetLookAt().z );
    incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Target:   %d,%d,%d",  (uint32)l_CameraLookAt.x, (uint32)l_CameraLookAt.y, (uint32)l_CameraLookAt.z );
    //incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, l_FontColor, "Type:   %s", pCamera->GetTypeStr().c_str());
}
