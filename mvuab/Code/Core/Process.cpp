#include "Process.h"
CProcess::CProcess() : m_pCamera( 0 ), m_FPS( 0 )
{
}

CProcess::CProcess( CCamera* ap_Camera ) : m_pCamera( ap_Camera ), m_FPS( 0 )
{
}

void CProcess::RenderDebugInfo()
{
}