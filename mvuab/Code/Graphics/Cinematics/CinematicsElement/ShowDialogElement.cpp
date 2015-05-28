#include "Cinematics\CinematicsElement\ShowDialogElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CShowDialogElement::CShowDialogElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Text( atts.GetPszProperty( "text", "" ) ) 
{
}

void CShowDialogElement::Execute( CGraphicsManager& GM )
{
	
}

void CShowDialogElement::Render(CGraphicsManager &GM)
{
}

void CShowDialogElement::Render2D(CGraphicsManager &GM)
{
}