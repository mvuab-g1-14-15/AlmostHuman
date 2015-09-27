#include "BillboardCore.h"
#include "BillboardInstance.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "GraphicsManager.h"

#include "RenderableVertex\VertexTypes.h"
#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "Effects/EffectManager.h"
#include "EngineManagers.h"
#include "EngineConfig.h"

CBillboardCore::CBillboardCore()
    : CName()
    , m_Texture( 0 )
    , mTechnique( 0 )
    , mSize(1.0f)
    , mAlpha(1.0f)
    , mFlipUVHorizontal( false )
    , mFlipUVVertical( false )
{
}

CBillboardCore::~CBillboardCore()
{
  Flush();
}

bool CBillboardCore::Init( const CXMLTreeNode& atts )
{
    SetName( atts.GetAttribute<std::string>( "name", "unknown" ) );
    m_Texture = atts.GetAttribute<CTexture>( "texture" );

    // Get the technique of the BillboardCore
    mTechnique        = atts.GetAttribute<CEffectTechnique>("technique");
    mSize             = atts.GetAttribute<float>( "size"              , 1.0f  );
    mAlpha            = atts.GetAttribute<float>( "alpha"             , 1.0f  );
    mFlipUVHorizontal = atts.GetAttribute<bool> ( "flip_uv_horizontal", false );
    mFlipUVVertical   = atts.GetAttribute<bool> ( "flip_uv_vertical"  , false );

    ASSERT( mTechnique, "Null technique %s to render the BillboardCore %s!", atts.GetAttribute<std::string>( "technique", "null_technique" ), GetName().c_str() );
    return ( mTechnique != 0 );
}

void CBillboardCore::Render( CRenderableVertexs* aRV, CGraphicsManager* aGM )
{
  if ( !mInstances.empty() )
  {
    if ( m_Texture )
      m_Texture->Activate( 0 );
    
    CEffect* lEffect = mTechnique->GetEffect();

    for( uint32 i = 0, lCount = mInstances.size(); i < lCount; ++i )
    {
      CEffect* lEffect = mTechnique->GetEffect();

      lEffect->SetSize( mSize );
      lEffect->SetAlpha( mAlpha );
      mInstances[i]->Render(aRV, aGM, mTechnique );
    }
  }
}

void CBillboardCore::AddInstance( CBillboardInstance* aInstance )
{
  if( aInstance )
  {
    mInstances.push_back( aInstance );
  }
}

void CBillboardCore::Flush()
{
  for( uint32 i = 0, lCount = mInstances.size(); i < lCount; ++i )
  {
    CHECKED_DELETE( mInstances[i] );
  }
  mInstances.clear();
}