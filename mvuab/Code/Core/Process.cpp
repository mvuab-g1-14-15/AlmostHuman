#include "Process.h"
CProcess::CProcess() : m_pCamera( 0 )
{
}

CProcess::CProcess( CCamera* ap_Camera ) : m_pCamera( ap_Camera )
{
}

void CProcess::RenderDebugInfo()
{
}