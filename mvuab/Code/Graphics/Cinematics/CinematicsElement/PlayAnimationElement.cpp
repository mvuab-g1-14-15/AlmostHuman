#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cinematics\Cinematic.h"
#include "Timer\Timer.h"

CPlayAnimationElement::CPlayAnimationElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
  , m_Cinematic( CCinematic(atts.GetPszProperty( "resource_id_file",  "no_file" ) ) )
  , m_Playing( false )
{
}

void CPlayAnimationElement::Update()
{
  if( !m_Playing )
  {
    m_Cinematic.Play(false);
    m_Playing = true;
  }
  m_Cinematic.Update();

  m_CurrentTime += deltaTimeMacro;
  if( m_CurrentTime >= m_Time )
  {
    m_IsBlocker = false;
  }
}

void CPlayAnimationElement::Execute( CGraphicsManager& GM )
{
  
}

void CPlayAnimationElement::Render()
{
  m_Cinematic.Render();
}

void CPlayAnimationElement::Render2D(CGraphicsManager &GM)
{
}