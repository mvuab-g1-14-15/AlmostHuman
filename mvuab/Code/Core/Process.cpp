#include "Process.h"
#include "Fonts\FontManager.h"
#include "GraphicsManager.h"
#include "Math\MathTypes.h"

CProcess::CProcess( )
	: m_pCamera( NULL ),
	m_FPS(0)
{

}

CProcess::CProcess( CCamera* ap_Camera )
	: m_pCamera( ap_Camera ),
	m_FPS(0)
{

}

void CProcess::RenderDebugInfo()
{
}