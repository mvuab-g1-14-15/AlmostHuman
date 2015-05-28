#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CPlayAnimationElement::CPlayAnimationElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Cinematic( atts.GetPszProperty( "resource_id_file",  "no_file" ) )
{
}

void CPlayAnimationElement::Execute( CGraphicsManager& GM )
{
}

void CPlayAnimationElement::Render(CGraphicsManager &GM)
{
}

void CPlayAnimationElement::Render2D(CGraphicsManager &GM)
{
}