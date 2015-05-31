#include "LensFlare.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

CLensFlare::CLensFlare( const CXMLTreeNode& node )
    : mColor( Math::CColor(1.0f, 1.0f, 1.0f, 1.0f ) )
{
    for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lNode = node(i);
        const std::string & lNodeName = lNode.GetName();
        if( lNodeName == "texture" )
        {
            CTexture* lTexture = TextureMInstance->GetTexture( lNode.GetPszProperty("name") );
            if( lTexture )
            {
                mTextures.push_back(lTexture);
            }
        }
    }
}

CLensFlare::~CLensFlare()
{
}