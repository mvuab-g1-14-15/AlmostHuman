#include "Cinematics\CinematicsElement\SetTransformElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CSetTransformElement::CSetTransformElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f(0,0,0) ) )
{
}

void CSetTransformElement::Execute( CGraphicsManager& GM )
{

}

void CSetTransformElement::Render(CGraphicsManager &GM)
{
}

void CSetTransformElement::Render2D(CGraphicsManager &GM)
{
}