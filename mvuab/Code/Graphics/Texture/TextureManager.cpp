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
}

void CTextureManager::Init()
{
	m_DummyTexture = new CTexture();
	if ( !m_DummyTexture->Load("Data/textures/Dummy.png") )
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
    if( fileName == "" )
    {
        return m_DummyTexture;
    }
    else if ( m_Resources.find( fileName ) == m_Resources.end() )
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

        if ( !t->Load( fileName ) )
        {
            CHECKED_DELETE( t );
            LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );
            return m_DummyTexture;
        }

        m_Resources[fileName] = t;
        return t;
    }
    else
    {
        return m_Resources[fileName];
    }
}
