#include "XMLTreeNode.h"
#include "Assert.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"

// Defines
#define MY_ENCODING "ISO-8859-1"

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode::CXMLTreeNode( const pugi::xml_node& aPugiNode )
    : mTreeNode( aPugiNode )
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode::~CXMLTreeNode()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::FirstChild()
{
    return CXMLTreeNode( mTreeNode.first_child() );
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::NextChild()
{
    return CXMLTreeNode( mTreeNode.next_sibling());
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::LastChild()
{
    return CXMLTreeNode( mTreeNode.last_child() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool CXMLTreeNode::More()
{
    return (mTreeNode) ? true : false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
bool CXMLTreeNode::IsOk() const
{
    return (mTreeNode) ? true : false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template< typename T > bool CXMLTreeNode::GetAttribute( const char* aAttName, T& aValue ) const
{
    ASSERT(false, "Not instanced type");
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template< typename T > T CXMLTreeNode::GetAttribute( const char* aAttName ) const
{
    ASSERT(false, "Not instanced type");
    return T();
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<bool>( const char* aAttName, bool& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    aValue = false;

    if(lAttribute)
    {
        aValue = ( strcmp( "TRUE", lAttribute ) == 0 ||
                   strcmp( "true", lAttribute ) == 0 ||
                   strcmp( "True", lAttribute ) == 0 );
    }

    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<bool>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    bool lValue = false;

    if(lAttribute)
    {
        lValue = ( strcmp( "TRUE", lAttribute ) == 0 ||
                   strcmp( "true", lAttribute ) == 0 ||
                   strcmp( "True", lAttribute ) == 0 );
    }

    return lValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<std::string>( const char* aAttName, std::string& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    aValue = (lAttribute) ? lAttribute : "";
    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> std::string CXMLTreeNode::GetAttribute<std::string>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    std::string lValue = (lAttribute) ? lAttribute : "";
    return lValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<Math::Vect2i>( const char* aAttName, Math::Vect2i& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    if( lAttribute )
    {
        sscanf_s( lAttribute, "%d %d", &aValue.x, &aValue.y );
    }
    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> Math::Vect2i CXMLTreeNode::GetAttribute<Math::Vect2i>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();
    Math::Vect2i lValue;
    if( lAttribute )
    {
        sscanf_s( lAttribute, "%d %d", &lValue.x, &lValue.y );
    }
    return lValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<Math::Vect3i>( const char* aAttName, Math::Vect3i& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    if( lAttribute )
    {
        sscanf_s( lAttribute, "%d %d %d", &aValue.x, &aValue.y, &aValue.z );
    }

    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> Math::Vect3i CXMLTreeNode::GetAttribute<Math::Vect3i>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();
    Math::Vect3i lValue;
    if( lAttribute )
    {
        sscanf_s( lAttribute, "%d %d %d", &lValue.x, &lValue.y, &lValue.z );
    }
    return lValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<Math::Vect2f>( const char* aAttName, Math::Vect2f& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    if( lAttribute )
    {
        sscanf_s( lAttribute, "%f %f", &aValue.x, &aValue.y );
    }

    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> Math::Vect2f CXMLTreeNode::GetAttribute<Math::Vect2f>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();
    Math::Vect2f lValue;
    if( lAttribute )
    {
        sscanf_s( lAttribute, "%f %f", &lValue.x, &lValue.y );
    }
    return lValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> bool CXMLTreeNode::GetAttribute<Math::Vect3f>( const char* aAttName, Math::Vect3f& aValue ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();

    if( lAttribute )
    {
        sscanf_s( lAttribute, "%f %f %f", &aValue.x, &aValue.y, &aValue.z );
    }

    return (lAttribute != 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> Math::Vect3f CXMLTreeNode::GetAttribute<Math::Vect3f>( const char* aAttName ) const
{
    const char* lAttribute = mTreeNode.attribute(aAttName).value();
    Math::Vect3f lValue;
    if( lAttribute )
    {
        sscanf_s( lAttribute, "%f %f %f", &lValue.x, &lValue.y, &lValue.z );
    }
    return lValue;
}