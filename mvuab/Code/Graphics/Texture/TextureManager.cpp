#include "TextureManager.h"

CTextureManager::CTextureManager()
    : CManager()
{
}

CTextureManager::CTextureManager( CXMLTreeNode& atts )
    : CManager( atts )
{
}

CTextureManager::~CTextureManager()
{
    Destroy();
    CHECKED_DELETE( m_DummyTexture );
}

void CTextureManager::Init()
{
    m_DummyTexture = new CTexture();
    if ( !m_DummyTexture->Load(sDummyTextureName) )
    {
        CHECKED_DELETE( m_DummyTexture );
        LOG_ERROR_APPLICATION( "The Dummy texture could not be loaded" );
        m_DummyTexture = 0;
    }
}

void CTextureManager::Reload()
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();

    for ( ; itb != ite; ++itb )
    {
        itb->second->Reload();
    }
}

CTexture* CTextureManager::GetTexture( const std::string& fileName )
{
    if( fileName == ""  )
    {
        ASSERT( false, "Null texture name" );
        return m_DummyTexture;
    }

    //TMapResource::iterator l_It = m_Resources.find(fileName);
    //return (l_It == m_Resources.end()) ? AddTexture( fileName ) : l_It->second;

    CTexture *t = GetResource(fileName);
    return (t == NULL) ? AddTexture(fileName) : t;
}

CTexture* CTextureManager::AddTexture( const std::string& fileName )
{
    CTexture* t;

    if ( fileName.find( "Cube" ) != std::string::npos )
    {
        t = new CCubedTexture();
    }
    else if ( fileName.find( "GUI" ) != std::string::npos )
    {
        t = new CGUITexture();
    }
    else
    {
        t = new CTexture();
    }
    
    if ( !t->Load( fileName ) || !AddResource( fileName, t) )
    {
        CHECKED_DELETE( t );
        LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );

        t = GetResource(fileName);
        return t == NULL ? m_DummyTexture : t;
    }

    return t;
}
