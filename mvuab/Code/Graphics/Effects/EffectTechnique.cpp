#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"

#include "Timer\Timer.h"
#include "Logger\Logger.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

#define READ_TECHNIQUE_FLAG( node, attribute, flag ) flag = node.GetAttribute<bool>( attribute, false );
#define CHECK_FLAG( flag ) ( flag )

CEffectTechnique::CEffectTechnique( const CXMLTreeNode& aTechniqueNode )
    : CName( aTechniqueNode.GetAttribute<std::string>("name", "null") )
    , m_DebugColor( Math::colWHITE )
    , m_VertexType( aTechniqueNode.GetAttribute<uint32>("vertex_type", 0 ) )
    , m_Filename( aTechniqueNode.GetAttribute<std::string>("file", "null_file") )
    , mUseWorld                 ( true )
    , mUseInverseWorld          ( true )
    , mUseView                  ( true )
    , mUseInverseView           ( true )
    , mUseProjection            ( true )
    , mUseInverseProjection     ( true )
    , mUseViewProjection        ( true )
    , mUseWorldView             ( true )
    , mUseWorldViewProjection   ( true )
    , mUseViewToLightProjection ( true )
    , mUseFog                   ( true )
    , mUseLights                ( true )
    , mUseDebugColor            ( true )
    , mUseFBSize                ( true )
    , mUseAmbientLight          ( true )
    , mUseCamera                ( true )
    , mUseTime                  ( true )
{
  ASSERT( m_Filename != "null_file", "Check the file of the technique %s", GetName().c_str() );

  //Use the compiled effect
  m_Filename = "../CommonData/fx/" + GetName() + ".o";

  LOG_INFO_APPLICATION( "Loading effect tecnhique %s", m_Filename.c_str() );

  for ( uint32 j = 0, lCount = aTechniqueNode.GetNumChildren(); j < lCount; ++j )
  {
    const CXMLTreeNode& lCurrentNode = aTechniqueNode( j );
    const std::string& l_TagName = lCurrentNode.GetName();

    if ( l_TagName == "handles" )
    {
      ReadFlags( lCurrentNode );
    }
#ifdef _DEBUG
    if( l_TagName == "define" )
    {
      SDefines lDefine = { lCurrentNode.GetAttribute<std::string>("name", "null_name"), lCurrentNode.GetAttribute<std::string>("description", "") };
      m_Defines.push_back(lDefine);
    }
#endif
  }

#ifdef _DEBUG
  // By default insert the definition of the technique name
  SDefines lDefine = { "TECHNIQUE_NAME", GetName() };
  m_Defines.push_back(lDefine);
#endif

  m_Effect = new CEffect(GetName() + "_Effect" );
  bool lLoaded = m_Effect->Load( m_Filename, m_Defines );
  ASSERT(lLoaded, "The effect %s could not be loaded" );

  if( lLoaded )
  {
    m_D3DTechnique = ( m_Effect ) ? m_Effect->GetTechniqueByName( GetName() ) : 0;
  }
  else
  {
    CHECKED_DELETE( m_Effect );
  }
}

CEffectTechnique::~CEffectTechnique()
{
  CHECKED_DELETE( m_Effect );
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
        m_Effect->SetDebugColor( mUseDebugColor, m_DebugColor );

        if ( mUseFBSize )
        {
          uint32 lWidth, lHeight;
          GraphicsInstance->GetWidthAndHeight( lWidth, lHeight );
          if( !m_Effect->SetFBSize( Math::Vect2u(lWidth, lHeight ) ) )
            LOG_WARNING_APPLICATION("Error setting Fb size" );
        }

        SetupMatrices();
        SetupLights();
    }

    return true;
}

bool CEffectTechnique::Refresh()
{
    m_Effect = new CEffect(GetName() + "_Effect" );
    bool lLoaded = m_Effect->Load( m_Filename, m_Defines );
    ASSERT(lLoaded, "The effect %s could not be loaded" );

    if( lLoaded )
    {
        m_D3DTechnique = ( m_Effect ) ? m_Effect->GetTechniqueByName( GetName() ) : 0;
    }
    else
    {
        CHECKED_DELETE( m_Effect );
    }

    return lLoaded;
}

void CEffectTechnique::SetupMatrices()
{
    CEffectManager* l_pEffectManager = EffectManagerInstance;

    // Obtain the matrices only once then multiply or invert depending on the case
    const Math::Mat44f lProjectionMtx = l_pEffectManager->GetProjectionMatrix();
    const Math::Mat44f lViewMtx = l_pEffectManager->GetViewMatrix();
    const Math::Mat44f lWorldMtx = l_pEffectManager->GetWorldMatrix();

    if ( CHECK_FLAG( mUseWorld ) )
    {
        if ( !m_Effect->SetWorldMatrix( lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION("Error Setting World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseWorldView ) )
    {
        if ( !m_Effect->SetWorldViewMatrix( lViewMtx * lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting World-View Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseWorldViewProjection ) )
    {
        if ( !m_Effect->SetWorldViewProjectionMatrix( lProjectionMtx*lViewMtx*lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting World-View-Projection Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseViewToLightProjection ) )
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

    if ( CHECK_FLAG( mUseInverseWorld ) )
    {
        if ( !m_Effect->SetInverseWorldMatrix( lWorldMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseProjection ) )
    {
        if ( !m_Effect->SetProjectionMatrix( lProjectionMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseView ) )
    {
        if ( !m_Effect->SetViewMatrix( lViewMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseViewProjection ) )
    {
        if ( !m_Effect->SetViewProjectionMatrix( lProjectionMtx * lViewMtx) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseInverseProjection ) )
    {
        if ( !m_Effect->SetInverseProjectionMatrix( lProjectionMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseInverseView ) )
    {
        if ( !m_Effect->SetInverseViewMatrix( lViewMtx ) )
        {
            LOG_WARNING_APPLICATION( "Error Setting Inverse World Matrix" );
        }
    }

    if ( CHECK_FLAG( mUseCamera ) )
    {
      if( !m_Effect->SetCameraPosition( EffectManagerInstance->GetCameraEye() ) )
      {
        LOG_ERROR_APPLICATION( "Error binding the camera to the shared effect!!" );
      }
    }

    if ( CHECK_FLAG( mUseAmbientLight ) )
    {
      if( !m_Effect->SetAmbientLightColor( LightMInstance->GetAmbientLight() ) )
      {
        LOG_ERROR_APPLICATION( "Error binding the ambient light to the shared effect!!" );
      }
    }

    if ( CHECK_FLAG( mUseTime ) )
    {
      if( !m_Effect->SetDeltaTime( constFrameTime ) )
      {
        LOG_ERROR_APPLICATION( "Error binding the delta time to the shared effect!!" );
      }
    }

    if ( CHECK_FLAG( mUseFBSize ) )
    {
      Math::Vect2u lFBSize;
      GraphicsInstance->GetWidthAndHeight(lFBSize.x, lFBSize.y);
      if( !m_Effect->SetFBSize( lFBSize ) )
      {
        LOG_ERROR_APPLICATION( "Error binding the fb size to the shared effect!!" );
      }
    }
}

bool CEffectTechnique::SetupLights()
{
    if ( CHECK_FLAG( mUseLights ) )
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

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsIntensityParameter(),
          &m_Effect->GetLightsIntensity()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
          LOG_WARNING_APPLICATION( "SetupLights->Error Setting lights intensity" );
        }

        if ( l_Effect->SetFloatArray( m_Effect->GetLightsSpecularIntensityParameter(),
          &m_Effect->GetLightsSpecularIntensity()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
        {
          LOG_WARNING_APPLICATION( "CEffectTechnique::SetupLights->Error Setting specular intensity" );
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

void CEffectTechnique::ReadFlags( const CXMLTreeNode& aFlagsNode )
{
    m_NumOfLights   = aFlagsNode.GetAttribute<int32>( "num_of_lights", 0 );

    // Read the flags of the technique from the handles node
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_world_matrix", mUseWorld );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_inverse_world_matrix", mUseInverseWorld );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_view_matrix", mUseView );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_inverse_view_matrix", mUseInverseView );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_projection_matrix", mUseProjection );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_inverse_projection_matrix", mUseInverseProjection );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_view_projection_matrix", mUseViewProjection );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_world_view_matrix", mUseWorldView );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_world_view_projection_matrix", mUseWorldViewProjection );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_view_to_light_projection_matrix", mUseViewToLightProjection );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_fog", mUseFog );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_lights", mUseLights );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_debug_color", mUseDebugColor );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_frame_buffer_size", mUseFBSize );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_light_ambient_color", mUseAmbientLight );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_camera_position", mUseCamera );
    READ_TECHNIQUE_FLAG( aFlagsNode, "use_time", mUseTime );
}