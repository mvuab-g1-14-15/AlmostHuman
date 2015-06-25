#include "XMLTreeNode.h"
#include "Assert.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"

// Defines
#define MY_ENCODING "ISO-8859-1"

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode::CXMLTreeNode()
    : m_bIsOk( false )
    , m_pDoc( NULL )
    , m_pNode( NULL )
    , m_pWriter( NULL )
    , m_pszFileName( NULL )
{}

//-----------------------------------------------------------------------------------------------------------------------------------------
CXMLTreeNode::~CXMLTreeNode()
{
    Done();
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CXMLTreeNode::Done()
{
    if ( IsOk() )
    {
        Release();
        m_bIsOk = false;
    }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CXMLTreeNode::Release()
{
    if ( m_pDoc )
    {
        xmlFreeDoc( m_pDoc );
        m_pDoc = NULL;
    }

    m_pNode = NULL;

    if ( m_pWriter )
    {
        xmlFreeTextWriter( m_pWriter );
        m_pWriter = NULL;
    }

    xmlCleanupParser();
}

bool CXMLTreeNode::LoadAndFindNode( const char* aFilename, const char* aNodeTag, CXMLTreeNode& aNode )
{
    ASSERT(aFilename && aNodeTag, "Invalid filename or node tag");
    CXMLTreeNode newFile;

    if ( !LoadFile( aFilename ) )
    {
        LOG_ERROR_APPLICATION( "Unable to open %s",  aFilename );
        return false;
    }

    // Parse the file and search for the key's
    aNode = GetNode(aNodeTag);

    if ( !aNode.Exists() )
    {
        LOG_ERROR_APPLICATION( "Unable to find %s",  aNodeTag );
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Load File
//----------------------------------------------------------------------------
bool CXMLTreeNode::LoadFile( const char* _pszFileName )
{
    m_bIsOk = false;
    ASSERT( _pszFileName, "Null filename" );
    if ( _pszFileName )
    {
        m_pDoc = xmlParseFile( _pszFileName );

        xmlErrorPtr lError = xmlGetLastError();
        ASSERT( !lError, "%s in file %s", ( const char* ) lError->message, _pszFileName );

        if ( m_pDoc )
        {
            m_pNode = xmlDocGetRootElement( m_pDoc );

            if ( m_pNode )
            {
                m_bIsOk = true;
                return true;
            }
        }
    }

    Release();
    return false;
}

//----------------------------------------------------------------------------
// Returns a subtree node from a given key
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::GetSubTree( const char* _pszKey ) const
{
    assert( m_pNode && _pszKey );
    CXMLTreeNode NewTree;

    if ( _pszKey && m_pNode )
    {
        _FindSubTree( m_pNode, _pszKey, NewTree );
    }

    return NewTree;
}

//----------------------------------------------------------------------------
// Recursive to find a key on a tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::_FindSubTree( xmlNodePtr _pNode, const char* _pszKey,
                                 CXMLTreeNode& _TreeFound ) const
{
    while ( _pNode != NULL )
    {
        if ( xmlStrcmp( _pNode->name, ( const xmlChar* )_pszKey ) )
        {
            if ( _FindSubTree( _pNode->xmlChildrenNode, _pszKey, _TreeFound ) )
            {
                return true;
            }
        }
        else
        {
            _TreeFound.m_pNode = _pNode;
            _TreeFound.m_pDoc = m_pDoc;
            return true;
        }

        _pNode = _pNode->next;
    }

    return false;
}

//----------------------------------------------------------------------------
// Returns the main node keyword
//----------------------------------------------------------------------------
const char* CXMLTreeNode::GetName() const
{
    if ( m_pNode )
    {
        return ( char* )m_pNode->name;
    }

    return NULL;
}

//----------------------------------------------------------------------------
// Operator that returns a tree node
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator[]( const char* _pszKey ) const
{
    assert( _pszKey && m_pNode );
    CXMLTreeNode TreeFound;

    if ( _pszKey && m_pNode )
    {
        TreeFound = GetSubTree( _pszKey );
    }

    return TreeFound;
}

CXMLTreeNode CXMLTreeNode::GetNode( const char* _pszKey ) const
{
    assert( _pszKey && m_pNode );
    CXMLTreeNode TreeFound;

    if ( _pszKey && m_pNode )
    {
        TreeFound = GetSubTree( _pszKey );
    }

    return TreeFound;
}

//----------------------------------------------------------------------------
// Operator to get children nodes
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator()( int _iIndex ) const
{
    assert( _iIndex >= 0 && m_pNode );
    CXMLTreeNode TreeFound;

    if ( _iIndex >= 0 && m_pNode )
    {
        int iCount = 0;
        xmlNodePtr pChildren = m_pNode->children;

        while ( pChildren != NULL )
        {
            if ( pChildren->type != XML_TEXT_NODE && pChildren->type != XML_COMMENT_NODE )
            {
                if ( _iIndex == iCount )
                {
                    TreeFound.m_pNode = pChildren;
                    TreeFound.m_pDoc = m_pDoc;
                    break;
                }

                ++iCount;
            }

            pChildren = pChildren->next;
        }
    }

    return TreeFound;
}

CXMLTreeNode CXMLTreeNode::GetChildren( int _iIndex ) const
{
    assert( _iIndex >= 0 && m_pNode );
    CXMLTreeNode TreeFound;

    if ( _iIndex >= 0 && m_pNode )
    {
        int iCount = 0;
        xmlNodePtr pChildren = m_pNode->children;

        while ( pChildren != NULL )
        {
            if ( pChildren->type != XML_TEXT_NODE && pChildren->type != XML_COMMENT_NODE )
            {
                if ( _iIndex == iCount )
                {
                    TreeFound.m_pNode = pChildren;
                    TreeFound.m_pDoc = m_pDoc;
                    break;
                }

                ++iCount;
            }

            pChildren = pChildren->next;
        }
    }

    return TreeFound;
}

//----------------------------------------------------------------------------
// Returns the number of children a tree has
//----------------------------------------------------------------------------
uint32 CXMLTreeNode::GetNumChildren() const
{
    assert( m_pNode );
    int iCount = 0;

    if ( m_pNode )
    {
        xmlNodePtr pChildren = m_pNode->children;

        while ( pChildren != NULL )
        {
            if ( pChildren->type != XML_TEXT_NODE && pChildren->type != XML_COMMENT_NODE )
            {
                ++iCount;
            }

            pChildren = pChildren->next;
        }
    }

    return iCount;
}

//----------------------------------------------------------------------------
// Returns a param of the tree from a given key
//----------------------------------------------------------------------------
xmlChar* CXMLTreeNode::GetProperty( const char* _pszKey ) const
{
    assert( _pszKey && m_pNode );
    xmlChar* value = NULL;

    if ( _pszKey && m_pNode )
    {
        value = xmlGetProp( m_pNode, ( const xmlChar* )_pszKey );
    }

    return value;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const bool CXMLTreeNode::GetAttribute<bool>( const char* aAttName, const bool& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    bool lRtn( aDefaultAttValue );

    if ( value )
    {
        const char* pszValue = ( const char* )value;

        lRtn = ( strcmp( "TRUE", pszValue ) == 0 ||
                 strcmp( "true", pszValue ) == 0 ||
                 strcmp( "True", pszValue ) == 0 );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const int32 CXMLTreeNode::GetAttribute<int32>( const char* aAttName, const int32& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    int32 lRtn( aDefaultAttValue );

    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%d", &lRtn );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const uint32 CXMLTreeNode::GetAttribute<uint32>( const char* aAttName, const uint32& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    uint32 lRtn( aDefaultAttValue );

    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%u", &lRtn );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const float32 CXMLTreeNode::GetAttribute<float32>( const char* aAttName,
        const float32& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    float32 lRtn( aDefaultAttValue );

    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%f", &lRtn );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const std::string CXMLTreeNode::GetAttribute<std::string>( const char* aAttName,
        const std::string& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    std::string lRtn( aDefaultAttValue );

    if ( value )
    {
        lRtn = ( const char* )value;
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::CColor CXMLTreeNode::GetAttribute<Math::CColor>( const char* aAttName,
        const Math::CColor& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::CColor lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%f %f %f %f", &lRtn.r, &lRtn.g, &lRtn.b, &lRtn.a );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::Vect2f CXMLTreeNode::GetAttribute<Math::Vect2f>( const char* aAttName,
        const Math::Vect2f& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::Vect2f lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%f %f", &lRtn.x, &lRtn.y );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::Vect2i CXMLTreeNode::GetAttribute<Math::Vect2i>( const char* aAttName,
        const Math::Vect2i& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::Vect2i lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%d %d", &lRtn.x, &lRtn.y );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::Vect2u CXMLTreeNode::GetAttribute<Math::Vect2u>( const char* aAttName,
        const Math::Vect2u& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::Vect2u lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%u %u", &lRtn.x, &lRtn.y );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::Vect4f CXMLTreeNode::GetAttribute<Math::Vect4f>( const char* aAttName,
        const Math::Vect4f& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::Vect4f lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%f %f %f %f", &lRtn.x, &lRtn.y, &lRtn.z, &lRtn.w );
    }

    xmlFree( value );
    return lRtn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> const Math::Vect3f CXMLTreeNode::GetAttribute<Math::Vect3f>( const char* aAttName,
        const Math::Vect3f& aDefaultAttValue ) const
{
    xmlChar* value = GetProperty( aAttName );

    Math::Vect3f lRtn( aDefaultAttValue );
    if ( value )
    {
        const char* c_strValue = ( const char* )value;
        sscanf_s( c_strValue, "%f %f %f", &lRtn.x, &lRtn.y, &lRtn.z );
    }

    xmlFree( value );
    return lRtn;
}

//----------------------------------------------------------------------------
// Returns an keyword from the tree from a given key
//----------------------------------------------------------------------------
xmlChar* CXMLTreeNode::GetKeyword( const char* _pszKey ) const
{
    assert( _pszKey && m_pNode && m_pDoc );
    xmlChar* value = NULL;

    if ( _pszKey && m_pNode && m_pDoc )
    {
        CXMLTreeNode FoundTree;

        if ( _FindSubTree( m_pNode, _pszKey, FoundTree ) )
        {
            value = xmlNodeListGetString( FoundTree.m_pDoc, FoundTree.m_pNode->xmlChildrenNode, 1 );
        }
    }

    return value;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
bool CXMLTreeNode::ExistsKey( const char* _pszKey )
{
    assert( _pszKey );
    CXMLTreeNode TreeFound = GetSubTree( _pszKey );
    return TreeFound.Exists();
}

//----------------------------------------------------------------------------
bool CXMLTreeNode::StartNewFile( const char* _pszFileName )
{
    assert( _pszFileName );
    m_bIsOk = false;

    if ( _pszFileName )
    {
        m_pszFileName = _pszFileName;

        // Create a new XmlWriter for DOM, with no compression.
        m_pWriter = xmlNewTextWriterDoc( &m_pDoc, 0 );
        assert( m_pWriter );

        if ( m_pWriter )
        {
            // Start the document with the xml default for the version, encoding ISO 8858-1 and the default for the standalone declaration.
            int rc = xmlTextWriterStartDocument( m_pWriter, NULL, MY_ENCODING, NULL );
            assert( rc >= 0 );

            if ( rc >= 0 )
            {
                m_bIsOk = true;
                return true;
            }
        }
    }

    Release();
    return false;
}


//----------------------------------------------------------------------------
// Finished a file and saves it
//----------------------------------------------------------------------------
void CXMLTreeNode::EndNewFile()
{
    assert( m_pWriter && m_pDoc && m_pszFileName );

    if ( m_pWriter && m_pDoc && m_pszFileName )
    {
        xmlFreeTextWriter( m_pWriter );
        m_pWriter = NULL;
        xmlSaveFileEnc( m_pszFileName, m_pDoc, MY_ENCODING );
    }
}

//----------------------------------------------------------------------------
// Write a comment in the xml file in the current node
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteComment( const char* _pszComment )
{
    assert( _pszComment && m_pWriter );

    if ( _pszComment && m_pWriter )
    {
        int rc = xmlTextWriterWriteComment( m_pWriter, BAD_CAST _pszComment );
        assert( rc >= 0 );

        if ( rc < 0 )
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::StartElement( const char* _pszKey )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        // Start an element named "EXAMPLE". Since thist is the first element, this will be the root element of the document.
        int rc = xmlTextWriterStartElement( m_pWriter, BAD_CAST _pszKey );
        assert( rc >= 0 );

        if ( rc < 0 )
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::EndElement()
{
    assert( m_pWriter );

    if ( m_pWriter )
    {
        /* Close the element named HEADER. */
        int rc = xmlTextWriterEndElement( m_pWriter );
        assert( rc >= 0 );

        if ( rc < 0 )
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
// Writes a string keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszKeyword( const char* _pszKey, const char* _pszValue )
{
    assert( _pszKey && _pszValue && m_pWriter );

    if ( _pszKey && _pszValue && m_pWriter )
    {
        // Write an element
        int rc = xmlTextWriterWriteElement( m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes an integer keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntKeyword( const char* _pszKey, int _iValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%d", _iValue );
        // Write an element
        int rc = xmlTextWriterWriteElement( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a float32 keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatKeyword( const char* _pszKey, float32 _fValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%d", _fValue );
        // Write an element
        int rc = xmlTextWriterWriteElement( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a boolean keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolKeyword( const char* _pszKey, bool _bValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%d", _bValue ? "true" : "false" );
        // Write an element
        int rc = xmlTextWriterWriteElement( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a string property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszProperty( const char* _pszKey, const char* _pszValue )
{
    assert( _pszKey && _pszValue && m_pWriter );

    if ( _pszKey && _pszValue && m_pWriter )
    {
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes an integer property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntProperty( const char* _pszKey, int _iValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%d", _iValue );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a float32 property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatProperty( const char* _pszKey, float32 _fValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%f", _fValue );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a boolean property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolProperty( const char* _pszKey, bool _bValue )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, _bValue ? "true" : "false" );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}


//----------------------------------------------------------------------------
// Writes a Math::Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect2fProperty( const char* _pszKey, const Math::Vect2f& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%f %f", _value.x, _value.y );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a Math::Vect3f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect3fProperty( const char* _pszKey, const Math::Vect3f& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%f %f %f", _value.x, _value.y, _value.z );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a Math::Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect4fProperty( const char* _pszKey, const Math::Vect4f& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%f %f %f %f", _value.x, _value.y, _value.z, _value.w );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a Math::Vect2i property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect2iProperty( const char* _pszKey, const Math::Vect2i& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%i %i", _value.x, _value.y );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a Math::Vect3i property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect3iProperty( const char* _pszKey, const Math::Vect3i& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%i %i %i", _value.x, _value.y, _value.z );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Writes a Math::Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect4iProperty( const char* _pszKey, const Math::Vect4i& _value )
{
    assert( _pszKey && m_pWriter );

    if ( _pszKey && m_pWriter )
    {
        char pszValue[32];
        sprintf_s( pszValue, "%i %i %i %i", _value.x, _value.y, _value.z, _value.w );
        int rc = xmlTextWriterWriteAttribute( m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue );

        if ( rc >= 0 )
        {
            return true;
        }
    }

    return false;
}