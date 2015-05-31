#include "ShadowMap.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

CShadowMap::CShadowMap( const CXMLTreeNode& node )
    : mColor( Math::CColor(1.0f, 1.0f, 1.0f, 1.0f ) )
    , mTexture( 0 )
{

}

CShadowMap::~CShadowMap()
{
}