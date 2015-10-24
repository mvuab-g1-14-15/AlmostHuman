#include "Light.h"
#include "LensFlare/LensFlareManager.h"
#include "LensFlare/LensFlare.h"
#include "ShadowMap.h"

#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "GraphicsManager.h"
#include "Math\Color.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "Texture\Texture.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include <string>
#include "Billboard/BillboardInstance.h"

CLight::CLight()
    : CObject3D()
    , CName( "" )
    , m_StartRangeAttenuation( 0 )
    , m_EndRangeAttenuation( 0 )
    , m_Color( Math::Vect3f( 1, 1, 1 ) )
    , m_Intensity( 0 )
    , m_RenderShadows( 0 )
    , m_ViewShadowMap( Math::m44fIDENTITY )
    , m_ProjectionShadowMap( Math:: m44fIDENTITY )
    , m_ShadowMaskTexture( 0 )
    , mLensFlare( 0 )
    , mStaticShadowMap( 0 )
    , mDynamicShadowMap( 0 )
    , m_MustUpdateStaticShadowMap( false )
{
}

CLight::CLight( const CXMLTreeNode& node )
    : CObject3D( node ), CName( node.GetAttribute<std::string>( "name", "" ) )
    , m_StartRangeAttenuation( node.GetAttribute<float>( "att_start_range", 0 ) )
    , m_EndRangeAttenuation( node.GetAttribute<float>( "att_end_range", 0 ) )
    , m_Color( node.GetAttribute<Math::Vect3f>( "color", Math::Vect3f( 1, 1, 1 ) ) )
    , m_Intensity( node.GetAttribute<float>( "intensity", 0 ) )
    , m_RenderShadows( node.GetAttribute<bool>( "render_shadows", false ) )
    , m_ViewShadowMap( Math::m44fIDENTITY )
    , m_ProjectionShadowMap( Math:: m44fIDENTITY )
    , m_ShadowMaskTexture( 0 )
    , mLensFlare( 0 )
    , mStaticShadowMap( 0 )
    , mDynamicShadowMap( 0 )
    , m_RoomName()
    , mBillboard(0)
{
    ASSERT( m_Color.GetRed() <= 1.0f && m_Color.GetGreen() <= 1.0f &&
            m_Color.GetBlue() <= 1.0f, "Normalized Color for light %s", GetName().c_str() );

    for ( int i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
    {
        const CXMLTreeNode& lNode = node( i );
        const std::string& TagName = lNode.GetName();

        if ( TagName == "lens_flare" )
        { 
            ASSERT( !mLensFlare, "The light %s only could have one lens flare", GetName().c_str() );
            mLensFlare = FlareMan->GetResource( lNode.GetAttribute<std::string>( "name", "null_flare" ) );
        }
        else if ( TagName == "shadow_map" )
        {
            m_RenderShadows = lNode.GetAttribute<bool>( "render_shadows", false );

            const std::string& l_ShadowMaskTextureFile = lNode.GetAttribute<std::string>( "shadow_texture_mask", "" );

            m_RoomName = lNode.GetAttribute<std::string>( "room", "" );

            if ( l_ShadowMaskTextureFile != "" )
            {
                m_ShadowMaskTexture = TextureMInstance->GetTexture( l_ShadowMaskTextureFile );
            }

            for ( uint32 iShadows = 0, lShadowsCount = lNode.GetNumChildren(); iShadows < lShadowsCount; ++iShadows )
            {
                const CXMLTreeNode& lShadowNode = lNode( iShadows );
                const std::string& ShadowType = lShadowNode.GetName();

                if ( ShadowType == "static_shadow_map" )
                {
                    ASSERT( !mStaticShadowMap, "The light %s only could have one static shadow map", GetName().c_str() );
                    mStaticShadowMap = new CShadowMap( lShadowNode, m_RoomName );
                    m_MustUpdateStaticShadowMap = true;
                }
                else if ( ShadowType == "dynamic_shadow_map" )
                {
                    ASSERT( !mDynamicShadowMap, "The light %s only could have one dynamic shadow map", GetName().c_str() );
                    mDynamicShadowMap = new CShadowMap( lShadowNode, m_RoomName );
                }
            }
        }
    }
}

CLight::~CLight()
{
    CHECKED_DELETE( mDynamicShadowMap );
    CHECKED_DELETE( mStaticShadowMap );
}

void CLight::SetIntensity( const float intensity )
{
    m_Intensity = intensity;
}

float CLight::GetIntensity() const
{
    return m_Intensity;
}

void CLight::SetType( const ELightType Type )
{
    m_Type = Type;
}

void CLight::SetColor( const Math::CColor& Color )
{
    m_Color = Color;
}

const Math::CColor& CLight::GetColor() const
{
    return m_Color;
}

void CLight::SetStartRangeAttenuation( const float StartRangeAttenuation )
{
    m_StartRangeAttenuation = StartRangeAttenuation;
}

float CLight::GetStartRangeAttenuation() const
{
    return m_StartRangeAttenuation;
}

void CLight::SetEndRangeAttenuation( const float EndRangeAttenuation )
{
    m_EndRangeAttenuation = EndRangeAttenuation;
}

float CLight::GetEndRangeAttenuation() const
{
    return m_EndRangeAttenuation;
}

CLight::ELightType CLight::GetType() const
{
    return m_Type;
}

void CLight::SetMustUpdateStaticShadowMap( bool MustUpdateStaticShadowMap )
{
    m_MustUpdateStaticShadowMap = MustUpdateStaticShadowMap;
}

bool CLight::GetMustUpdateStaticShadowMap() const
{
    return m_MustUpdateStaticShadowMap;
}

CShadowMap *CLight::GetStaticShadowMap() const
{
    return mStaticShadowMap;
}

CShadowMap *CLight::GetDynamicShadowMap() const
{
    return mDynamicShadowMap;
}

CTexture *CLight::GetShadowMaskTexture() const
{
    return m_ShadowMaskTexture;
}

void CLight::GenerateShadowMap( CGraphicsManager *GM )
{
    if ( m_RenderShadows )
    {
        SetShadowMap( GM );

        if ( m_MustUpdateStaticShadowMap && mStaticShadowMap)
        {
            mStaticShadowMap->Generate();
            m_MustUpdateStaticShadowMap = false;
        }

        if( mDynamicShadowMap )
          mDynamicShadowMap->Generate();
    }
}

const Mat44f& CLight::GetViewShadowMap() const
{
    return m_ViewShadowMap;
}

const Mat44f& CLight::GetProjectionShadowMap() const
{
    return m_ProjectionShadowMap;
}

void CLight::BeginRenderEffectManagerShadowMap( CEffect *Effect )
{
    if ( m_RenderShadows )
    {
        CEffectManager *l_EM = EffectManagerInstance;
        l_EM->SetLightViewMatrix( m_ViewShadowMap );
        l_EM->SetShadowProjectionMatrix( m_ProjectionShadowMap );

        bool lUseStaticShadowMap( false ), lUseDynamicShadowMap( false ), lUseMaskShadowMap( false );

        if ( m_ShadowMaskTexture )
        {
            m_ShadowMaskTexture->Activate( SHADOW_MAP_MASK_STAGE );
            lUseMaskShadowMap = true;
        }

        if ( mStaticShadowMap )
        {
            lUseStaticShadowMap = mStaticShadowMap->Activate();
        }

        if ( mDynamicShadowMap )
        {
            lUseDynamicShadowMap = mDynamicShadowMap->Activate();
        }

        Effect->SetShadowMapParameters( lUseMaskShadowMap, lUseStaticShadowMap, lUseDynamicShadowMap );
    }
}