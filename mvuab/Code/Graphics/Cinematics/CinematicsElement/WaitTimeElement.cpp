#include "Cinematics\CinematicsElement\WaitTimeElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"

CWaitTimeElement::CWaitTimeElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Time( atts.GetFloatProperty( "time", 0.0 ) )
{
}

void CWaitTimeElement::Execute( CGraphicsManager& GM )
{
  float lCurrentTime = 0;
  while( m_Time>lCurrentTime )
  {
    lCurrentTime += deltaTimeMacro;
  }
}

void CWaitTimeElement::Render(CGraphicsManager &GM)
{
}

void CWaitTimeElement::Render2D(CGraphicsManager &GM)
{
}