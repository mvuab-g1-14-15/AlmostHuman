#include "TextureManager.h"

#include <sys\stat.h>
#include "Utils\BaseUtils.h"

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
  CTexture* lTexture = (Exist( fileName )) ? m_Resources[fileName] : AddTexture(fileName);
  return (lTexture) ? lTexture : m_DummyTexture;
}

bool CTextureManager::TryToLoad(CTexture* aTexture, std::string aFileName)
{
    return (baseUtils::fileExists(aFileName)) && aTexture->Load( aFileName );
}

CTexture* CTextureManager::AddTexture( const std::string& fileName )
{
    CTexture* t = ( fileName.find( "Cube" ) != std::string::npos ) ? new CCubedTexture() : new CTexture();

    std::string lFileName = fileName;
    lFileName.erase(lFileName.find_last_of("."), std::string::npos);

    // Try with dds files
    bool lOk = TryToLoad( t, lFileName + ".dds");

    // Load the not compressed texture
    if (!lOk)
      lOk = !TryToLoad( t, fileName);

    // Check if something has gone wrong
    if ( !lOk || !AddResource( fileName, t) )
    {
        CHECKED_DELETE( t );
        LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );
    }

    return t;
}
