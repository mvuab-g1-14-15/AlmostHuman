#include "Cinematics\CinematicsElement\CinematicsElement.h"

CCinematicsElement::CCinematicsElement( CXMLTreeNode& atts )
  : m_IsBlocker( atts.GetBoolProperty("blocker", false) )
  , m_Time( atts.GetFloatProperty("time", 0.0f) )
  , m_CurrentTime( 0 )
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