#include "Cinematics\CinematicsElement\CinematicsElement.h"

CCinematicsElement::CCinematicsElement( CXMLTreeNode& atts )
{
  if ( atts.Exists() )
  {
    SetName( atts.GetPszProperty( "name", atts.GetName() ) );
    SetVisible( atts.GetBoolProperty( "active", false ) );
  }
}

CCinematicsElement::~CCinematicsElement()
{
}