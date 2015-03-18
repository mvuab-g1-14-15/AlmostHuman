#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Core.h"
#include "Timer\Timer.h"
#include "Logger\Logger.h"
#include "GraphicsManager.h"

CEffectTechnique::CEffectTechnique( const std::string& TechniqueName, const std::string& EffectName,
                                    CXMLTreeNode& HandlesNode )
  : m_TechniqueName( TechniqueName ),
    m_EffectName( EffectName ),
    m_UseCameraPosition( HandlesNode.GetBoolProperty( "use_camera_position", false ) ),
    m_UseInverseProjMatrix( HandlesNode.GetBoolProperty( "use_inverse_projection_matrix", false ) ),
    m_UseInverseViewMatrix( HandlesNode.GetBoolProperty( "use_inverse_view_matrix", false ) ),
    m_UseInverseWorldMatrix( HandlesNode.GetBoolProperty( "use_inverse_world_matrix", false ) ),


    m_UseProjMatrix( HandlesNode.GetBoolProperty( "use_projection_matrix", false ) ),
    m_UseViewMatrix( HandlesNode.GetBoolProperty( "use_view_matrix", false ) ),
    m_UseWorldMatrix( HandlesNode.GetBoolProperty( "use_world_matrix", false ) ),
    m_UseWorldViewMatrix( HandlesNode.GetBoolProperty( "use_world_view_matrix", false ) ),
    m_UseWorldViewProjectionMatrix( HandlesNode.GetBoolProperty( "use_world_view_projection_matrix",
                                    false ) ),
    m_UseViewProjectionMatrix( HandlesNode.GetBoolProperty( "use_view_projection_matrix", false ) ),
    m_UseViewToLightProjectionMatrix(
      HandlesNode.GetBoolProperty( "use_view_to_light_projection_matrix", false ) ),

    // Lights
    m_NumOfLights( HandlesNode.GetIntProperty( "num_of_lights", 0 ) ),
    m_UseLights( HandlesNode.GetBoolProperty( "use_lights", false ) ),
    m_UseLightAmbientColor( HandlesNode.GetBoolProperty( "use_light_ambient_color", false ) ),

    // Debug
    m_UseDebugColor( HandlesNode.GetBoolProperty( "use_debug_color", false ) ),
    m_DebugColor( Math::colWHITE ),

    // Timers
    m_UseTime( HandlesNode.GetBoolProperty( "use_time", false ) ),
    m_UseDeltaTime( HandlesNode.GetBoolProperty( "use_delta_time", false ) ),

    // Fog
    m_FogStart( HandlesNode.GetFloatProperty( "fog_start", 0 ) ),
    m_FogEnd( HandlesNode.GetFloatProperty( "fog_end", 0 ) ),
    m_FogExp( HandlesNode.GetFloatProperty( "fog_exp", 0 ) ),
    m_FogFun( HandlesNode.GetIntProperty( "fog_fun", 1 ) ),

    // Texture
    m_UseTextureSizes( HandlesNode.GetBoolProperty( "use_texture_size", false ) ),
    m_TextureHeight( HandlesNode.GetIntProperty( "texture_height", 0 ) ),
    m_TextureWidth( HandlesNode.GetIntProperty( "texture_width", 0 ) )
{
  m_Effect = CEffectManager::GetSingletonPtr()->GetEffect( m_EffectName );
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
    return false;

  // Obtain the direct x effect
  LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
  D3DXHANDLE l_Handle = NULL;

  if ( m_UseCameraPosition )
  {
    Math::Vect3f l_CameraEye = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetPos();
    m_Effect->SetCameraPosition( l_CameraEye );
  }

  m_Effect->SetUseDebugColor(false);
  if ( m_UseDebugColor )
  {
    l_Handle = m_Effect->GetDebugColor();
    float32 l_DebugColor[4];
    l_DebugColor[0] = m_DebugColor.GetRed();
    l_DebugColor[1] = m_DebugColor.GetGreen();
    l_DebugColor[2] = m_DebugColor.GetBlue();
    l_DebugColor[3] = m_DebugColor.GetAlpha();
    l_Effect->SetFloatArray( l_Handle, l_DebugColor, sizeof( float ) * 4 );
	m_Effect->SetUseDebugColor(true);
  }

  if ( m_FogFun == 2 )
  {
    l_Handle = m_Effect->GetFogStart();
    l_Effect->SetFloat( l_Handle, m_FogStart );

    l_Handle = m_Effect->GetFogEnd();
    l_Effect->SetFloat( l_Handle, m_FogEnd );

    l_Handle = m_Effect->GetFogFun();
    l_Effect->SetInt( l_Handle, m_FogFun );
  }

  if ( m_FogFun == 1 || m_FogFun == 3 )
  {
    l_Handle = m_Effect->GetFogExp();
    l_Effect->SetFloat( l_Handle, m_FogExp );

    l_Handle = m_Effect->GetFogFun();
    l_Effect->SetInt( l_Handle, m_FogFun );
  }

  if ( m_UseTextureSizes )
  {
    l_Handle = m_Effect->GetHeightTexture();
    l_Effect->SetInt( l_Handle, m_TextureHeight );

    l_Handle = m_Effect->GetWidthTexture();
    l_Effect->SetInt( l_Handle, m_TextureWidth );
  }

  if ( m_UseTime )
  {
    l_Handle = m_Effect->GetTimeParameter();
    l_Effect->SetFloat( l_Handle, CCore::GetSingletonPtr()->GetTimer()->GetTime() );
  }

  if ( m_UseDeltaTime )
  {
    l_Handle = m_Effect->GetDeltaTimeParameter();
    l_Effect->SetFloat( l_Handle, CCore::GetSingletonPtr()->GetTimer()->GetElapsedTime() );
  }

  SetupMatrices();
  SetupLights();
  return true;
}

bool CEffectTechnique::Refresh()
{
  m_Effect = CEffectManager::GetSingletonPtr()->GetEffect( m_EffectName );
  m_D3DTechnique = m_Effect->GetTechniqueByName( m_TechniqueName );
  return ( m_Effect != 0 && m_D3DTechnique != 0 );
}

void CEffectTechnique::SetupMatrices()
{
  CEffectManager* l_pEffectManager = CEffectManager::GetSingletonPtr();

  if ( m_UseProjMatrix )
  {
    if ( !m_Effect->SetProjectionMatrix( l_pEffectManager->GetProjectionMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting Projection Matrix" );
  }

  if ( m_UseViewMatrix )
  {
    if ( !m_Effect->SetViewMatrix( l_pEffectManager->GetViewMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting View Matrix" );
  }

  if ( m_UseWorldMatrix )
  {
    if ( !m_Effect->SetWorldMatrix( l_pEffectManager->GetWorldMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting World Matrix" );
  }

  if ( m_UseWorldViewMatrix )
  {
    if ( !m_Effect->SetWorldViewMatrix( l_pEffectManager->GetViewMatrix()
                                        *l_pEffectManager->GetWorldMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting World-View Matrix" );
  }

  if ( m_UseWorldViewProjectionMatrix )
  {
    if ( !m_Effect->SetWorldViewProjectionMatrix( l_pEffectManager->GetProjectionMatrix()
         *l_pEffectManager->GetViewMatrix()*l_pEffectManager->GetWorldMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting World-View-Projection Matrix" );
  }

  if ( m_UseViewProjectionMatrix )
  {
    if ( !m_Effect->SetViewProjectionMatrix( l_pEffectManager->GetProjectionMatrix()
         *l_pEffectManager->GetViewMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting View-Projection Matrix" );
  }

  if ( m_UseViewToLightProjectionMatrix )
  {
    Math::Mat44f l_ViewToLightProjectionMatrix = l_pEffectManager->GetViewMatrix();
    l_ViewToLightProjectionMatrix.Invert();
	l_ViewToLightProjectionMatrix.SetIdentity();

	//l_ViewToLightProjectionMatrix = l_pEffectManager->GetLightViewMatrix()*l_pEffectManager->GetShadowProjectionMatrix();
	l_ViewToLightProjectionMatrix = l_pEffectManager->GetShadowProjectionMatrix()*l_pEffectManager->GetLightViewMatrix();
    
    /*l_ViewToLightProjectionMatrix = l_pEffectManager->GetShadowProjectionMatrix() *
                                    l_ViewToLightProjectionMatrix;

	l_ViewToLightProjectionMatrix = l_pEffectManager->GetLightViewMatrix() *
                                    l_ViewToLightProjectionMatrix;*/



		/*Mat44f l_ViewToLightProjectionMatrix=l_pEffectManager->GetViewMatrix();
		l_ViewToLightProjectionMatrix.Invert();
		l_ViewToLightProjectionMatrix=l_ViewToLightProjectionMatrix*l_pEffectManager->GetLightViewMatrix();
		l_ViewToLightProjectionMatrix=l_ViewToLightProjectionMatrix*l_pEffectManager->GetShadowProjectionMatrix();*/
//l_Effect->SetMatrix(m_Effect->m_ViewToLightProjectionMatrixParameter, &l_ViewToLightProjectionMatrix.GetD3DXMatrix());


    if ( !m_Effect->SetViewToLightMatrix( l_ViewToLightProjectionMatrix ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting View To Light Matrix" );
  }

  if ( m_UseInverseProjMatrix )
  {
    if ( !m_Effect->SetInverseProjectionMatrix( l_pEffectManager->GetProjectionMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting Inverse Projection Matrix" );
  }

  if ( m_UseInverseViewMatrix )
  {
    if ( !m_Effect->SetInverseViewMatrix( l_pEffectManager->GetViewMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting Inverse View Matrix" );
  }

  if ( m_UseInverseWorldMatrix )
  {
    if ( !m_Effect->SetInverseWorldMatrix( l_pEffectManager->GetWorldMatrix() ) )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupMatrices->Error Setting Inverse World Matrix" );
  }
}

bool CEffectTechnique::SetupLights()
{
  if ( m_UseLights )
  {
    LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
    CEffectManager* l_pEffectManager = CEffectManager::GetSingletonPtr();

    // Check the number of lights
    // If the technique is rendering a deferred shader the lights has been set up
    // in the command, therefore check here the light number in order to not reset the values
    if ( m_NumOfLights > 1 )
    {
      if ( !m_Effect->SetLights( m_NumOfLights ) )
      {
        CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
                                               "CEffectTechnique::SetupLights->Setting the num of lights to %d", m_NumOfLights );
        return false;
      }
    }

    if ( l_Effect->SetBoolArray( m_Effect->GetLightEnabledParameter(), &m_Effect->GetLightsEnabled()[0],
                                 MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights enabled" );

    if ( l_Effect->SetIntArray( m_Effect->GetLightsTypeParameter(), m_Effect->GetLightsType(),
                                MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights types" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsAngleParameter(), m_Effect->GetLightsAngle(),
                                  MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights  angle parameters" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsFallOffParameter(), m_Effect->GetLightsFallOff(),
                                  MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights fall of parameters" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsStartRangeAttenuationParameter(),
                                  &m_Effect->GetLightsStartRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting start range" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsEndRangeAttenuationParameter(),
                                  &m_Effect->GetLightsEndRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting end range" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsPositionParameter(),
                                  &m_Effect->GetLightsPosition()[0].x, MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights position" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsDirectionParameter(),
                                  &m_Effect->GetLightsDirection()[0].x, MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights position" );

    if ( l_Effect->SetFloatArray( m_Effect->GetLightsColorParameter(), &m_Effect->GetLightsColor()[0].x,
                                  MAX_LIGHTS_BY_SHADER * 3 ) != S_OK )
      CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING,
                                             "CEffectTechnique::SetupLights->Error Setting lights colors" );
  }

  if ( m_UseLightAmbientColor )
  {
    // Ask Jordi
  }

  return true;
}

void CEffectTechnique::SetDebugColor( Math::CColor color )
{
  m_DebugColor = color;
}