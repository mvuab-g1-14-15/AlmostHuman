#include "Cinematics\CinematicsElement\WaitTimeElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"

CWaitTimeElement::CWaitTimeElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
{
}

void CWaitTimeElement::Update()
{
  m_CurrentTime += deltaTimeMacro;
  if( m_CurrentTime >= m_Time )
  {
    m_IsBlocker = false;
  }
}

void CWaitTimeElement::Execute( CGraphicsManager& GM )
{
  
}

void CWaitTimeElement::Render()
{
}

void CWaitTimeElement::Render2D(CGraphicsManager &GM)
{
}