#include "XML/XMLTreeNode.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> CTexture* CXMLTreeNode::GetAttribute<CTexture>( const char* aAttName ) const
{
    xmlChar* value = GetProperty( aAttName );

    CTexture* lRtn = 0;

    if ( value )
    {
        const char* pszValue = ( const char* )value;
        lRtn = TextureMInstance->GetTexture(std::string(pszValue));
    }

    xmlFree( value );
    return lRtn;
}