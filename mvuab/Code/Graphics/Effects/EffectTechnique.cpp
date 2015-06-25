#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
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
      m_UseCameraPosition( HandlesNode.GetAttribute<bool>( "use_camera_position", false ) ),
      m_UseInverseProjMatrix( HandlesNode.GetAttribute<bool>( "use_inverse_projection_matrix", false ) ),
      m_UseInverseViewMatrix( HandlesNode.GetAttribute<bool>( "use_inverse_view_matrix", false ) ),
      m_UseInverseWorldMatrix( HandlesNode.GetAttribute<bool>( "use_inverse_world_matrix", false ) ),

      m_UseProjMatrix( HandlesNode.GetAttribute<bool>( "use_projection_matrix", false ) ),
      m_UseViewMatrix( HandlesNode.GetAttribute<bool>( "use_view_matrix", false ) ),
      m_UseWorldMatrix( HandlesNode.GetAttribute<bool>( "use_world_matrix", false ) ),
      m_UseWorldViewMatrix( HandlesNode.GetAttribute<bool>( "use_world_view_matrix", false ) ),
      m_UseWorldViewProjectionMatrix( HandlesNode.GetAttribute<bool>( "use_world_view_projection_matrix",
                                      false ) ),
      m_UseViewProjectionMatrix( HandlesNode.GetAttribute<bool>( "use_view_projection_matrix", false ) ),
      m_UseViewToLightProjectionMatrix(
          HandlesNode.GetAttribute<bool>( "use_view_to_light_projection_matrix", false ) ),

      // Lights
      m_NumOfLights( HandlesNode.GetAttribute<int32>( "num_of_lights", 0 ) ),
      m_UseLights( HandlesNode.GetAttribute<bool>( "use_lights", false ) ),
      m_UseLightAmbientColor( HandlesNode.GetAttribute<bool>( "use_light_ambient_color", false ) ),

      // Debug
      m_UseDebugColor( HandlesNode.GetAttribute<bool>( "use_debug_color", false ) ),
      m_DebugColor( Math::colWHITE ),

      // Timers
      m_UseTime( HandlesNode.GetAttribute<bool>( "use_time", false ) ),
      m_UseDeltaTime( HandlesNode.GetAttribute<bool>( "use_delta_time", false ) ),

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
    if ( !m_Effect )
    {
        return false;
    }

    // Obtain the direct x effect
    LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
    D3DXHANDLE l_Handle = NULL;

    //
    // Camera position
    //
    if ( m_UseCameraPosition )
    {
        Math::Vect3f l_CameraEye = CameraMInstance->GetCurrentCamera()->GetPosition();
        m_Effect->SetCameraPosition( l_CameraEye );
    }

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

    // TODO Check the time
    if ( m_UseTime )
    {
        l_Handle = m_Effect->GetTimeParameter();
        l_Effect->SetFloat( l_Handle, deltaTimeMacro );
    }

    if ( m_UseDeltaTime )
    {
        l_Handle = m_Effect->GetDeltaTimeParameter();
        l_Effect->SetFloat( l_Handle, deltaTimeMacro );
    }

    SetupMatrices();
    SetupLights();
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

    if ( m_UseProjMatrix )
    {
        if ( !m_Effect->SetProjectionMatrix( l_pEffectManager->GetProjectionMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting Projection Matrix" );
        }
    }

    if ( m_UseViewMatrix )
    {
        if ( !m_Effect->SetViewMatrix( l_pEffectManager->GetViewMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting View Matrix" );
        }
    }

    if ( m_UseWorldMatrix )
    {
        if ( !m_Effect->SetWorldMatrix( l_pEffectManager->GetWorldMatrix() ) )
            LOG_WARNING_APPLICATION(
                "CEffectTechnique::SetupMatrices->Error Setting World Matrix" );
    }

    if ( m_UseWorldViewMatrix )
    {
        if ( !m_Effect->SetWorldViewMatrix( l_pEffectManager->GetViewMatrix()
                                            *l_pEffectManager->GetWorldMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting World-View Matrix" );
        }
    }

    if ( m_UseWorldViewProjectionMatrix )
    {
        if ( !m_Effect->SetWorldViewProjectionMatrix( l_pEffectManager->GetProjectionMatrix()
                *l_pEffectManager->GetViewMatrix()*l_pEffectManager->GetWorldMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting World-View-Projection Matrix" );
        }
    }

    if ( m_UseViewProjectionMatrix )
    {
        if ( !m_Effect->SetViewProjectionMatrix( l_pEffectManager->GetProjectionMatrix()
                *l_pEffectManager->GetViewMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting View-Projection Matrix" );
        }
    }

    if ( m_UseViewToLightProjectionMatrix )
    {
        Math::Mat44f l_ViewToLightProjectionMatrix = l_pEffectManager->GetViewMatrix();
        l_ViewToLightProjectionMatrix.Invert();
        l_ViewToLightProjectionMatrix.SetIdentity();
        l_ViewToLightProjectionMatrix = l_pEffectManager->GetShadowProjectionMatrix() *
                                        l_pEffectManager->GetLightViewMatrix();

        if ( !m_Effect->SetViewToLightMatrix( l_ViewToLightProjectionMatrix ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting View To Light Matrix" );
        }
    }

    if ( m_UseInverseProjMatrix )
    {
        if ( !m_Effect->SetInverseProjectionMatrix( l_pEffectManager->GetProjectionMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting Inverse Projection Matrix" );
        }
    }

    if ( m_UseInverseViewMatrix )
    {
        if ( !m_Effect->SetInverseViewMatrix( l_pEffectManager->GetViewMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting Inverse View Matrix" );
        }
    }

    if ( m_UseInverseWorldMatrix )
    {
        if ( !m_Effect->SetInverseWorldMatrix( l_pEffectManager->GetWorldMatrix() ) )
        {
            LOG_WARNING_APPLICATION( "CEffectTechnique::SetupMatrices->Error Setting Inverse World Matrix" );
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

    if ( m_UseLightAmbientColor )
    {
        m_Effect->SetAmbientLightColor( LightMInstance->GetAmbientLight() );
    }

    return true;
}

void CEffectTechnique::SetDebugColor( Math::CColor color )
{
    m_DebugColor = color;
}