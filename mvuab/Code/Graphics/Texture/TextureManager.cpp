#include "TextureManager.h"

#include <sys\stat.h>

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
  CTexture* lTexture = NULL;

  TMapResource::iterator l_It = m_Resources.find(fileName);

  if( !Exist( fileName ) )
    lTexture = AddTexture( fileName );
  else
    lTexture = l_It->second;
    
  if( fileName == ""  )
  {
    ASSERT( false, "Null texture name" );
    lTexture = m_DummyTexture;
  }

  return lTexture;
}

bool fileExists(const std::string& file)
{
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

bool CTextureManager::TryToLoad(CTexture* aTexture, std::string aFileName)
{
    if (fileExists(aFileName))
    {
        if( aTexture->Load( aFileName ) )
        {
            return true;
        }
    }
    return false;
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

    std::string lFileName = fileName;
    lFileName.erase(lFileName.find_last_of("."), std::string::npos);
    if (!TryToLoad( t, lFileName + ".dds"))
    {
        if (!TryToLoad( t, fileName))
        {
            CHECKED_DELETE( t );
            LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );
            return m_DummyTexture;
        }
    }

    if ( !AddResource( fileName, t) )
    {
        CHECKED_DELETE( t );
        LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );
        return m_DummyTexture;
    }

    return t;
}
