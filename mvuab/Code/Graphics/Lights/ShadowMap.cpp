#include "ShadowMap.h"
#include "Texture/TextureManager.h"
#include "GraphicsManager.h"
#include "RenderableObject/RenderableObjectsLayersManager.h"
#include "RenderableObject/RenderableObjectsManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

//<shadow_map generate="true" layer ="static" renderable_objects_manager="solid" format_type="R32F" size="1024 1024"/>
CShadowMap::CShadowMap( const CXMLTreeNode& node )
    : mColor( Math::CColor(1.0f, 1.0f, 1.0f, 1.0f ) )
    , mTexture( new CTexture() )
    , mGenerate( node.GetBoolProperty("generate", false))
    , mLayer( node.GetPszProperty("layer", "static") )
    , mFormatType( CTexture::eR32F )
    , mSize( node.GetVect2iProperty("size", Math::Vect2i(0, 0) ) )
    , mStage( node.GetIntProperty("stage", 5 ) )
{
    mFormatType = mTexture->GetFormatTypeFromString( node.GetPszProperty( "format_type", "" ) );

    if( mGenerate )
    {
        mTexture->Create( std::string("Light_ShadowMap_") + mLayer, mSize.x, mSize.y, 1, CTexture::eUsageRenderTarget,
                          CTexture::eDefaultPool, mFormatType );
    }

    CRenderableObjectsManager* lROM = ROLMInstance->GetResource( node.GetPszProperty( "renderable_objects_manager", "" ) );

    if ( lROM )
    {
        mROMs.push_back(lROM);
    }

    mClearMask = ( mLayer == "static" ) ? 0x000000ff : 0xffffffff;
}

CShadowMap::~CShadowMap()
{
    CHECKED_DELETE(mTexture);
}

bool CShadowMap::Generate()
{
    if(mGenerate && !mROMs.empty())
    {
        CGraphicsManager* lGM = GraphicsInstance;
        // To write into the texture of the static shadow map
        mTexture->SetAsRenderTarget();
        lGM->BeginRender();
        lGM->Clear( true, true, false, mClearMask );

        for ( size_t i = 0, lROMSize = mROMs.size(); i < lROMSize; ++i )
        {
            mROMs[i]->Render();
        }

        lGM->EndRender();
        mTexture->UnsetAsRenderTarget();
        return true;
    }
    return false;
}

bool CShadowMap::Activate()
{
    if( mGenerate && !mROMs.empty() )
    {
        mTexture->Activate(mStage);
        return true;
    }
    return false;
}