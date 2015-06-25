#include "XML/XMLTreeNode.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Effects/EffectTechnique.h"
#include "Effects/EffectManager.h"

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

//-----------------------------------------------------------------------------------------------------------------------------------------
template<> CEffectTechnique* CXMLTreeNode::GetAttribute<CEffectTechnique>( const char* aAttName ) const
{
    xmlChar* value = GetProperty( aAttName );

    CEffectTechnique* lRtn = 0;

    if ( value )
    {
        const char* pszValue = ( const char* )value;
        lRtn = EffectManagerInstance->GetEffectTechnique(std::string(pszValue));
    }

    xmlFree( value );
    return lRtn;
}