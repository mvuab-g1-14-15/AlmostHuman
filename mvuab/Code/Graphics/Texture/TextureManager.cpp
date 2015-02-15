#include "TextureManager.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
  Destroy();
}

void CTextureManager::Reload()
{
  TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();

  for ( ; itb != ite; ++itb )
    itb->second->Reload();
}

CTexture* CTextureManager::GetTexture( const std::string fileName )
{
  if ( m_Resources.find( fileName ) == m_Resources.end() )
  {
    CTexture* t;

    if ( fileName.find( "Cube" ) != std::string::npos )
      t = new CCubedTexture();
    else
      t = new CTexture();

    if ( !t->Load( fileName ) )
    {
      CHECKED_DELETE( t );
      return 0;
    }

    m_Resources[fileName] = t;
    return t;
  }
  else
    return m_Resources[fileName];
}
