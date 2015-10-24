#include "ShadowMap.h"
#include "Texture/TextureManager.h"
#include "GraphicsManager.h"
#include "RenderableObject/RenderableObjectsLayersManager.h"
#include "RenderableObject/RenderableObjectsManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"

//<shadow_map generate="true" layer ="static" renderable_objects_manager="solid" format_type="R32F" size="1024 1024"/>
CShadowMap::CShadowMap( const CXMLTreeNode& node, const std::string& aRoomName )
    : mColor( Math::CColor(1.0f, 1.0f, 1.0f, 1.0f ) )
    , mTexture( new CTexture() )
    , mGenerate( node.GetAttribute<bool>("generate", false))
    , mLayer( node.GetAttribute<std::string>("renderable_objects_manager", "") )
    , mFormatType( CTexture::eR32F )
    , mSize( node.GetAttribute<Math::Vect2i>("size", Math::Vect2i(0, 0) ) )
    , mStage( node.GetAttribute<int32>("stage", 5 ) )
{
    mFormatType = mTexture->GetFormatTypeFromString( node.GetAttribute<std::string>( "format_type", "" ) );

    if( mGenerate )
    {
        mTexture->Create( std::string("Light_ShadowMap_") + mLayer, mSize.x, mSize.y, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, mFormatType );
    }
    mClearMask = ( node.GetAttribute<std::string>("layer", "static") == "static" ) ? 0x000000ff : 0xffffffff;
}

CShadowMap::~CShadowMap()
{
    CHECKED_DELETE(mTexture);
}

bool CShadowMap::Generate()
{
	bool lOk = false;
    if(mGenerate)
    {
        CGraphicsManager* lGM = GraphicsInstance;
        // To write into the texture of the static shadow map
        mTexture->SetAsRenderTarget();
        lGM->BeginRender();
        lGM->Clear( true, true, false, mClearMask );
        SceneInstance->RenderLayer(mLayer);
        lGM->EndRender();
        mTexture->UnsetAsRenderTarget();
        //mTexture->Save("ShadownMap");
        lOk = true;
    }
    return lOk;
}

bool CShadowMap::Activate()
{
    if( mGenerate )
    {
        mTexture->Activate(mStage);
        return true;
    }
    return false;
}