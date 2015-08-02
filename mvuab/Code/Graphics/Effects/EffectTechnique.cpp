#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "EffectFlags.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"

#include "Timer\Timer.h"
#include "Logger\Logger.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CEffectTechnique::CEffectTechnique( const std::string& TechniqueName, const std::string& EffectName,
                                    CXMLTreeNode& HandlesNode )
    : m_TechniqueName( TechniqueName ),
      m_EffectName( EffectName ),
      mFlags( new CEffectFlags( HandlesNode ) ),
      m_UseInverseWorldMatrix( HandlesNode.GetAttribute<bool>( "use_inverse_world_matrix", false ) ),
      m_UseWorldMatrix( HandlesNode.GetAttribute<bool>( "use_world_matrix", false ) ),
      m_UseWorldViewMatrix( HandlesNode.GetAttribute<bool>( "use_world_view_matrix", false ) ),
      m_UseWorldViewProjectionMatrix( HandlesNode.GetAttribute<bool>( "use_world_view_projection_matrix",
                                      false ) ),
      m_UseViewToLightProjectionMatrix(
          HandlesNode.GetAttribute<bool>( "use_view_to_light_projection_matrix", false ) ),

      // Lights
      m_NumOfLights( HandlesNode.GetAttribute<int32>( "num_of_lights", 0 ) ),
      m_UseLights( HandlesNode.GetAttribute<bool>( "use_lights", false ) ),

      // Debug
      m_UseDebugColor( HandlesNode.GetAttribute<bool>( "use_debug_color", false ) ),
      m_DebugColor( Math::colWHITE ),

      // Fog
      m_UseFog( HandlesNode.GetAttribute<bool>( "use_fog", false ) ),
      m_FogStart( HandlesNode.GetAttribute<float>( "fog_start", 0 ) ),
      m_FogEnd( HandlesNode.GetAttribute<float>( "fog_end", 0 ) ),
      m_FogExp( HandlesNode.GetAttribute<float>( "fog_exp", 0 ) ),
      m_FogFun( ( EFogFunction )HandlesNode.GetAttribute<int32>( "fog_fun", 1 ) ),

      // Texture
      m_UseTextureSizes( HandlesNode.GetAttribute<bool>( "use_texture_size", false ) ),
      m_TextureHeight( HandlesNode.GetAttribute<int32>( "texture_height", 0 ) ),
      m_TextureWidth( HandlesNode.GetAttribute<int32>( "texture_width", 0 ) )
{
    m_Effect = EffectManagerInstance->GetEffect( m_EffectName );
    m_D3DTechnique = ( m_Effect ) ? m_Effect->GetTechniqueByName( m_TechniqueName ) : 0;
}

CEffectTechnique::~CEffectTechnique()
{
    m_Effect = 0;
    CHECKED_DELETE(mFlags);
}

void CEffectTechnique::SetUseTextureSize( bool active )
{
    m_UseTextureSizes = active;
}

void CEffectTechnique::SetTextureSize( unsigned int width, unsigned int height )
{
    m_TextureHeight = height;
    m_TextureWidth = width;
}

bool CEffectTechnique::BeginRender()
{
    if ( m_Effect )
    {
        // Obtain the direct x effect
        LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
        D3DXHANDLE l_Handle = NULL;

        //
        // Debug color, only for debug primitives
        //
        m_Effect->SetDebugColor( m_UseDebugColor, m_DebugColor );
        //
        // Fog value
        //
        m_Effect->SetFog( m_UseFog, m_FogStart, m_FogEnd, m_FogExp, m_FogFun );

        if ( m_UseTextureSizes )
        {
            l_Handle = m_Effect->GetHeightTexture();
            l_Effect->SetInt( l_Handle, m_TextureHeight );
            l_Handle = m_Effect->GetWidthTexture();
            l_Effect->SetInt( l_Handle, m_TextureWidth );
        }

        SetupMatrices();
        SetupLights();
    }

    return true;
}

bool CEffectTechnique::Refresh()
{
    m_Effect = EffectManagerInstance->GetEffect( m_EffectName );
    m_D3DTechnique = m_Effect->GetTechniqueByName( m_TechniqueName );
    return ( m_Effect != 0 && m_D3DTechnique != 0 );
}

void CEffectTechnique::SetupMatrices()
{
    CEffectManager* l_pEffectManager = EffectManagerInstance;

    // Obtain the matrices only once then multiply or invert depending on the case
    const Math::Mat44f lProjectionMtx = l_pEffectManager->GetProjectionMatrix();
    const Math::Mat44f lViewMtx = l_pEffectManager->GetViewMatrix();
    const Math::Mat44f lWorldMtx = l_pEffectManager->GetWorldMatrix();

    if ( m_UseWorldMatrix )
    {
        if ( !m_Effect->SetWorldMatrix( lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION("Error Setting World Matrix" );
        }
    }

    if ( m_UseWorldViewMatrix )
    {
        if ( !m_Effect->SetWorldViewMatrix( lViewMtx * lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting World-View Matrix" );
        }
    }

    if ( m_UseWorldViewProjectionMatrix )
    {
        if ( !m_Effect->SetWorldViewProjectionMatrix( lProjectionMtx*lViewMtx*lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting World-View-Projection Matrix" );
        }
    }

    if ( m_UseViewToLightProjectionMatrix )
    {
        Math::Mat44f l_ViewToLightProjectionMatrix = lViewMtx;
        l_ViewToLightProjectionMatrix.Invert();
        l_ViewToLightProjectionMatrix.SetIdentity();
        l_ViewToLightProjectionMatrix = l_pEffectManager->GetShadowProjectionMatrix() *
                                        l_pEffectManager->GetLightViewMatrix();

        if ( !m_Effect->SetViewToLightMatrix( l_ViewToLightProjectionMatrix ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting View To Light Matrix" );
        }
    }

    if ( m_UseInverseWorldMatrix )
    {
        if ( !m_Effect->SetInverseWorldMatrix( lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }
}

bool CEffectTechnique::SetupLights()
{
    if ( m_UseLights )
    {
        LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
        CEffectManager* l_pEffectManager = EffectManagerInstance;

        // Check the number of lights
        // If the technique is rendering a deferred shader the lights has been set up
        // in the command, therefore check here the light number in order to not reset the values
        if ( m_NumOfLights > 1 )
        {
            if ( !m_Effect->SetLights( m_NumOfLights ) )
            {
                LOG_ERROR_APPLICATION( "CEffectTechnique::SetupLights->Setting the num of lights to %d",
                                       m_NumOfLights );
                return false;
            }
        }

        if ( l_Effect->SetBoolArray( m_Effect->GetLightEnabledParameter(), &m_Effect->GetLightsEnabled()[0],
                                     MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights enabled" );
        }

        if ( l_Effect->SetIntArray( m_Effect->GetLightsTypeParameter(), m_Effect->GetLightsType(),
                                    MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights types" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsAngleParameter(), m_Effect->GetLightsAngle(),
                                      MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights  angle parameters" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsFallOffParameter(), m_Effect->GetLightsFallOff(),
                                      MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights fall of parameters" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsStartRangeAttenuationParameter(),
                                      &m_Effect->GetLightsStartRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting start range" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsEndRangeAttenuationParameter(),
                                      &m_Effect->GetLightsEndRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting end range" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsPositionParameter(),
                                      &m_Effect->GetLightsPosition()[0].x, MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights position" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsDirectionParameter(),
                                      &m_Effect->GetLightsDirection()[0].x, MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights position" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsColorParameter(), &m_Effect->GetLightsColor()[0].x,
                                      MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting lights colors" );
        }
    }

    return true;
}

void CEffectTechnique::SetDebugColor( Math::CColor color )
{
    m_DebugColor = color;
}