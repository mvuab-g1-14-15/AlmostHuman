#include "LensFlare.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "RenderableVertex/VertexTypes.h"
#include "Fonts/FontManager.h"

//---------------------------------------------------------------------------------
CLensFlare::CLensFlare()
    : mTechnique(0)
{
}

//---------------------------------------------------------------------------------
CLensFlare::~CLensFlare()
{
    mFlares.clear();
}

//---------------------------------------------------------------------------------
bool CLensFlare::Init( const CXMLTreeNode& node )
{
    bool lOk = true;
    SetName( node.GetAttribute<std::string>("name", "no_name") );
    mTechnique = node.GetAttribute<CEffectTechnique>("technique");

    ASSERT(mTechnique, "Null technique to render the flares");
    for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lNode       = node(i);
        const std::string & lNodeName   = lNode.GetName();
        if( lNodeName == "flare" )
        {
            SFlare lFlare =
            {
                lNode.GetAttribute<float32>("distance", 0.0f),
                lNode.GetAttribute<float32>("size", 0.0f),
                lNode.GetAttribute<float32>("max_size", 0.0f),
                lNode.GetAttribute<float32>("alpha", 0.0f),
                lNode.GetAttribute<Math::Vect3f>("color", Math::Vect3f()),
                lNode.GetAttribute<CTexture>("texture"),
            };

            mFlares.push_back(lFlare);
        }
    }
    return true;
}


// Method extracted from Game Programming Gems Series Book 1 Chapter 5
void CLensFlare::Render
(
    const Math::Vect2u& aLightProjPos,
    const Math::Vect2u& aCameraProjPos,
    float lAspectRatio
)
{
    ASSERT( mTechnique, "Null technique");
    mTechnique->BeginRender();

    ASSERT( mTechnique->GetEffect(), "Null effect");

    LPD3DXEFFECT l_Effect = mTechnique->GetEffect()->GetEffect();

    if ( l_Effect != NULL )
    {
        CGraphicsManager* lGM = GraphicsInstance;

        UINT l_NumPasses = 0;
        l_Effect->SetTechnique( mTechnique->GetD3DTechnique() );
        lGM->GetDevice()->SetVertexDeclaration(SCREEN_COLOR_VERTEX::GetVertexDeclaration());

        //compute how far off-center the flare source is
        uint32 lMaxFlareDistance = aCameraProjPos.Length();
        uint32 lFlareDistance  = (aLightProjPos-aCameraProjPos).Length();

        // determine overall scaling based on off-center distance
        float lDistanceScale = 1.0f;
        //float(lMaxFlareDistance - lFlareDistance ) / float( lMaxFlareDistance);

        // Flare is rendered along a line from aLightProjPos to a point opposite it across the center point.
        uint32 lDx = aCameraProjPos.x + (aCameraProjPos.x - aLightProjPos.x);
        uint32 lDy = aCameraProjPos.y + (aCameraProjPos.y - aLightProjPos.y);

        for( uint32 i = 0, lFlaresCount = mFlares.size(); i < lFlaresCount; ++i )
        {
            const SFlare lCurrentFlare = mFlares[i];

            // Position is interpolated between lightpos and camerapos
            float lPx = ( 1 - lCurrentFlare.mDistance ) * aLightProjPos.x + lCurrentFlare.mDistance * lDx;
            float lPy = ( 1 - lCurrentFlare.mDistance ) * aLightProjPos.y + lCurrentFlare.mDistance * lDy;

            // Size of the flare depends on its scale, distance scaling, and overall scale of the flare
            float lWidth = lCurrentFlare.mSize * lDistanceScale; // * lCurrentFlare.mScale;

            // Width gets clamped, so the off-axis flares keep a good size without letting the centered elements to big
            if( lWidth > lCurrentFlare.mMaxSize )
                lWidth = lCurrentFlare.mMaxSize;

            // Flare elements are squared so height is just width scaled by aspect ratio
            float lHeight = lWidth * lAspectRatio;

            // Alpha is based on element alpha and the distance scale
            float lAlpha = lCurrentFlare.mAlpha * lDistanceScale;

            mTechnique->GetEffect()->SetAlpha(lAlpha);
            mTechnique->GetEffect()->SetDebugColor(true,Math::CColor( lCurrentFlare.mColor.x, lCurrentFlare.mColor.y ,lCurrentFlare.mColor.z, lAlpha ));

            // Obtain the rect in screen space of the flare
            long left     = ( long )( lPx - lWidth );
            long top      = ( long )( lPy - lHeight );
            long bottom   = ( long )( lPy + lHeight );
            long right    = ( long )( lPx + lWidth );
            RECT l_Rect = { left, top, right, bottom };

            l_Effect->Begin( &l_NumPasses, 0 );

            for ( UINT iPass = 0; iPass < l_NumPasses; iPass++ )
            {
                l_Effect->BeginPass( iPass );
                lGM->DrawColoredQuad2DTexturedInPixels
                (
                    l_Rect,
                    Math::CColor( lCurrentFlare.mColor.x, lCurrentFlare.mColor.y ,lCurrentFlare.mColor.z, lAlpha ),
                    lCurrentFlare.mTexture,
                    0.0f, 0.0f, 1.0f, 1.0f );
                l_Effect->EndPass();
            }

            l_Effect->End();
        }
    }
}