#include "SceneRenderComands\BloomSceneRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "GraphicsManager.h"

#include "EngineManagers.h"
#include "Math\Color.h"

CBloomSceneRendererCommand::CBloomSceneRendererCommand( CXMLTreeNode& atts )
    : CStagedTexturedRendererCommand( atts )
    , m_Technique( atts.GetAttribute<CEffectTechnique>( "technique" ) )
    , mBloomThreshold( atts.GetAttribute<float>( "BloomThreshold", 0.5f ) )
    , mBlurAmount( atts.GetAttribute<float>( "BlurAmount", 4.0f ) )
    , mBloomIntensity( atts.GetAttribute<float>( "BloomIntensity", 0.8f ) )
    , mBloomOriginalIntensity( atts.GetAttribute<float>( "BloomOriginalIntensity", 1.0f ) )
    , mBloomSaturation( atts.GetAttribute<float>( "BloomSaturation", 1.5f ) )
    , mBloomOriginalSaturation( atts.GetAttribute<float>( "BloomOriginalSaturation", 1.0f ) )
    , CTOR_EFFECT_PARAMETER( BloomThreshold )
    , CTOR_EFFECT_PARAMETER( BloomIntensity )
    , CTOR_EFFECT_PARAMETER( BloomOriginalIntensity )
    , CTOR_EFFECT_PARAMETER( BloomSaturation )
    , CTOR_EFFECT_PARAMETER( BloomOriginalSaturation )
{
    // Get the technique and obtain the effect, then link the semantics
    ASSERT( m_Technique, "Null technique for bloom command" );
    m_Effect = m_Technique->GetEffect()->GetEffect();
    LinkBloomParameters();
}

void CBloomSceneRendererCommand::LinkBloomParameters()
{
    LINK_EFFECT_PARAMETER( BloomThreshold );
    LINK_EFFECT_PARAMETER( BloomIntensity );
    LINK_EFFECT_PARAMETER( BloomOriginalIntensity );
    LINK_EFFECT_PARAMETER( BloomSaturation );
    LINK_EFFECT_PARAMETER( BloomOriginalSaturation );
}

CBloomSceneRendererCommand::~CBloomSceneRendererCommand()
{
}

void CBloomSceneRendererCommand::Execute( CGraphicsManager& GM )
{
    ActivateTextures();

    for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
    {
        if ( m_StageTextures[i].mIsDynamic )
        {
            m_StageTextures[i].m_Texture->SetAsRenderTarget( m_StageTextures[i].m_StageId );
        }
    }

    if ( SET_FLOAT_PARAMETER( BloomThreshold, mBloomThreshold ) != S_OK )
    {
        LOG_ERROR_APPLICATION( "Could not set the bloom thresehold in the effect" );
    }

    if ( SET_FLOAT_PARAMETER( BloomIntensity, mBloomIntensity ) != S_OK )
    {
        LOG_ERROR_APPLICATION( "Could not set the bloom intensity in the effect" );
    }

    if ( SET_FLOAT_PARAMETER( BloomOriginalIntensity, mBloomOriginalIntensity ) != S_OK )
    {
        LOG_ERROR_APPLICATION( "Could not set the bloom original intensity in the effect" );
    }

    if ( SET_FLOAT_PARAMETER( BloomSaturation, mBloomSaturation ) != S_OK )
    {
        LOG_ERROR_APPLICATION( "Could not set the bloom saturation in the effect" );
    }

    if ( SET_FLOAT_PARAMETER( BloomOriginalSaturation, mBloomOriginalSaturation ) != S_OK )
    {
        LOG_ERROR_APPLICATION( "Could not set the bloom original saturation in the effect" );
    }

    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

    if ( m_Technique )
    {
        m_Technique->GetEffect()->SetLight( 0 );
        GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( m_Technique, l_Rect, Math::colWHITE,
                m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    }
    else
    {
        GM.DrawColoredQuad2DTexturedInPixels( l_Rect, Math::CColor(1, 1, 1, 0),
                                              m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    }

    for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
    {
        if ( m_StageTextures[i].mIsDynamic )
        {
            m_StageTextures[i].m_Texture->UnsetAsRenderTarget();
        }
    }

    DebugTextures();
}

void CBloomSceneRendererCommand::GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle )
{
    a_Handle = m_Effect->GetParameterBySemantic( 0, SemanticName );
    //ASSERT( a_Handle, "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName,
    //m_FileName.c_str() );
}