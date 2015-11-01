#include "Cinematics\CinematicsElement\CinematicsElement.h"

CCinematicsElement::CCinematicsElement( CXMLTreeNode& atts )
    : m_IsBlocker( atts.GetAttribute<bool>("blocker", false) )
    , m_Time( atts.GetAttribute<float>("time", 0.0f) )
    , m_CurrentTime( 0.0f )
{
    if ( atts.Exists() )
    {
        SetName( atts.GetAttribute<std::string>( "name", atts.GetName() ) );
        SetVisible( atts.GetAttribute<bool>( "active", false ) );
    }
}

CCinematicsElement::~CCinematicsElement()
{
}