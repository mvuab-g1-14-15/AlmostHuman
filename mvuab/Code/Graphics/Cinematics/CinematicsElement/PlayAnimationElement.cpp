#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cinematics\Cinematic.h"

CPlayAnimationElement::CPlayAnimationElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
  , m_Cinematic( CCinematic(atts.GetPszProperty( "resource_id_file",  "no_file" ) ) )
{
}

void CPlayAnimationElement::Update()
{
  m_Cinematic.Update();
}

void CPlayAnimationElement::Execute( CGraphicsManager& GM )
{
  m_Cinematic.Play(false);
}

void CPlayAnimationElement::Render(CGraphicsManager &GM)
{
  m_Cinematic.Render();
}

void CPlayAnimationElement::Render2D(CGraphicsManager &GM)
{
}