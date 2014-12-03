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
	uint32 incY = fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y,  colBLACK, "CAMERA");
	Vect3f cameraPosition( pCamera->GetEyePosition() );
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, colBLACK, "Position: %d,%d,%d", (uint32)cameraPosition.x, (uint32)cameraPosition.y,  (uint32)cameraPosition.z );
	Vect3f cameraLookAt( pCamera->GetLookAt() );
	incY += fm->DrawDefaultText(m_WindowPosition.x, m_WindowPosition.y + incY, colBLACK, "Target:   %d,%d,%d",  (uint32)cameraLookAt.x, (uint32)cameraLookAt.y,  (uint32)cameraLookAt.z );
}
