#include "Effect.h"
#include "EffectManager.h"
#include "EffectTechnique.h"
#include "GraphicsManager.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightManager.h"
#include "Lights/SpotLight.h"
#include "Logger\Logger.h"
#include "Utils/BaseUtils.h"
#include "Utils/StringUtils.h"

CEffect::CEffect()
  : m_FileName( "" ),
    m_Effect( 0 ),
    m_WorldMatrixParameter( 0 ),
    m_ViewMatrixParameter( 0 ),
    m_ProjectionMatrixParameter( 0 ),
    m_WorldViewMatrixParameter( 0 ),
    m_ViewProjectionMatrixParameter( 0 ),
    m_WorldViewProjectionMatrixParameter( 0 ),
    m_ViewToLightProjectionMatrixParameter( 0 ),
    m_LightEnabledParameter( 0 ),
    m_LightsTypeParameter( 0 ),
    m_LightsPositionParameter( 0 ),
    m_LightsDirectionParameter( 0 ),
    m_LightsAngleParameter( 0 ),
    m_LightsColorParameter( 0 ),
    m_LightsFallOffParameter( 0 ),
    m_LightsStartRangeAttenuationParameter( 0 ),
    m_LightsEndRangeAttenuationParameter( 0 ),
    m_CameraPositionParameter( 0 ),
    m_BonesParameter( 0 ),

    m_UseDebugColor( 0 ),
    m_DebugColor( 0 ),

    // Timers
    m_TimeParameter( 0 ),
    m_DeltaTimeParameter( 0 ),

    // Fog
    m_FogStart( 0 ),
    m_FogEnd( 0 ),
    m_FogExp( 0 ),
    m_FogFun( 0 ),

    // Texture
    m_HeightTexture( 0 ),
    m_WidthTexture( 0 )
{
  ResetLightsHandle();
}

CEffect::CEffect( const std::string& EffectName )
  : CName( EffectName ),
    m_FileName( "" ),
    m_Effect( 0 ),
    m_WorldMatrixParameter( 0 ),
    m_ViewMatrixParameter( 0 ),
    m_ProjectionMatrixParameter( 0 ),
    m_WorldViewMatrixParameter( 0 ),
    m_ViewProjectionMatrixParameter( 0 ),
    m_WorldViewProjectionMatrixParameter( 0 ),
    m_ViewToLightProjectionMatrixParameter( 0 ),
    m_LightEnabledParameter( 0 ),
    m_LightsTypeParameter( 0 ),
    m_LightsPositionParameter( 0 ),
    m_LightsDirectionParameter( 0 ),
    m_LightsAngleParameter( 0 ),
    m_LightsColorParameter( 0 ),
    m_LightsFallOffParameter( 0 ),
    m_LightsStartRangeAttenuationParameter( 0 ),
    m_LightsEndRangeAttenuationParameter( 0 ),
    m_CameraPositionParameter( 0 ),
    m_BonesParameter( 0 ),

    m_UseDebugColor( 0 ),
    m_DebugColor( 0 ),

    // Timers
    m_TimeParameter( 0 ),
    m_DeltaTimeParameter( 0 ),

    // Fog
    m_FogStart( 0 ),
    m_FogEnd( 0 ),
    m_FogExp( 0 ),
    m_FogFun( 0 ),

    // Texture
    m_HeightTexture( 0 ),
    m_WidthTexture( 0 )
{
  ResetLightsHandle();
}

CEffect::~CEffect()
{
  CHECKED_RELEASE( m_Effect );

  for ( size_t i = 0; i < m_NamesMacrosChar.size(); ++i )
    delete m_NamesMacrosChar[i];

  m_NamesMacrosChar.clear();

  for ( size_t i = 0; i < m_DescriptionsMacrosChar.size(); ++i )
    delete m_DescriptionsMacrosChar[i];

  m_DescriptionsMacrosChar.clear();

  m_Defines.clear();
}

void CEffect::SetNullParameters()
{
  // Reset the parameters
  m_WorldMatrixParameter = 0;
  m_ViewMatrixParameter = 0;
  m_ProjectionMatrixParameter = 0;
  m_WorldViewMatrixParameter = 0;
  m_ViewProjectionMatrixParameter = 0;
  m_WorldViewProjectionMatrixParameter = 0;
  m_ViewToLightProjectionMatrixParameter = 0;
  m_LightEnabledParameter = 0;
  m_LightsTypeParameter = 0;
  m_LightsPositionParameter = 0;
  m_LightsDirectionParameter = 0;
  m_LightsAngleParameter = 0;
  m_LightsColorParameter = 0;
  m_LightsFallOffParameter = 0;
  m_LightsStartRangeAttenuationParameter = 0;
  m_LightsEndRangeAttenuationParameter = 0;
  m_CameraPositionParameter = 0;
  m_BonesParameter = 0;

  // Timers
  m_TimeParameter = 0;
  m_DeltaTimeParameter = 0;

  // Fog
  m_FogStart = 0;
  m_FogEnd = 0;
  m_FogExp = 0;
  m_FogFun = 0;

  // Texture
  m_HeightTexture = 0;
  m_WidthTexture = 0;

  ResetLightsHandle();
}

void CEffect::GetParameterBySemantic( const std::string& SemanticName, D3DXHANDLE& a_Handle )
{
  a_Handle = m_Effect->GetParameterBySemantic( 0, SemanticName.c_str() );

  if ( !a_Handle )
  {
    LOG_ERROR_APPLICATION(
                                           "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName.c_str(),
                                           m_FileName.c_str() );
    assert( false );
  }
}

void CEffect::GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle )
{
  a_Handle = m_Effect->GetParameterBySemantic( 0, SemanticName );

  if ( !a_Handle )
  {
    LOG_ERROR_APPLICATION(
                                           "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName, m_FileName.c_str() );
    assert( false );
  }
}

bool CEffect::LoadEffect()
{
  // Obtain the device from the graphics manager and load the effect
  LPDIRECT3DDEVICE9 l_Device = CGraphicsManager::GetSingletonPtr()->GetDevice();
  DWORD dwShaderFlags = 0;
  dwShaderFlags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
  LPD3DXBUFFER l_ErrorBuffer = 0;
  HRESULT l_HR = D3DXCreateEffectFromFile(
                   l_Device,
                   m_FileName.c_str(),
                   &m_Defines[0], // [CONST D3DXMACRO* pDefines,] pDefines,
                   0, // LPD3DXINCLUDE pInclude,
                   dwShaderFlags,
                   0, // LPD3DXEFFECTPOOL pPool,
                   &m_Effect,
                   &l_ErrorBuffer );
  if ( l_ErrorBuffer )
  {
	  std::string lErrorMsg;
	  StringUtils::Format( lErrorMsg, "CEffect::Error creating effect '%s':\n%s", m_FileName.c_str(), l_ErrorBuffer->GetBufferPointer() );
	  LOG_ERROR_APPLICATION( lErrorMsg.c_str() );
	  ASSERT( l_HR == S_OK, lErrorMsg.c_str );
      CHECKED_RELEASE( l_ErrorBuffer );
      return false;
  }

  ASSERT( m_Effect, "Null Effect" );

  // Get the references to the handlers inside the effect
  GetParameterBySemantic( WorldMatrixParameterStr, m_WorldMatrixParameter );
  GetParameterBySemantic( ViewMatrixParameterStr, m_ViewMatrixParameter );
  GetParameterBySemantic( ProjectionMatrixParameterStr, m_ProjectionMatrixParameter );
  GetParameterBySemantic( InverseWorldMatrixParameterStr, m_InverseWorldMatrixParameter );
  GetParameterBySemantic( InverseViewMatrixParameterStr, m_InverseViewMatrixParameter );
  GetParameterBySemantic( InverseProjectionMatrixParameterStr, m_InverseProjectionMatrixParameter );
  GetParameterBySemantic( WorldViewMatrixParameterStr, m_WorldViewMatrixParameter );
  GetParameterBySemantic( ViewProjectionMatrixParameterStr, m_ViewProjectionMatrixParameter );
  GetParameterBySemantic( WorldViewProjectionMatrixParameterStr,
                          m_WorldViewProjectionMatrixParameter );
  GetParameterBySemantic( ViewToLightProjectionMatrixParameterStr,
                          m_ViewToLightProjectionMatrixParameter );
  GetParameterBySemantic( LightEnabledParameterStr, m_LightEnabledParameter );
  GetParameterBySemantic( LightsTypeParameterStr, m_LightsTypeParameter );
  GetParameterBySemantic( LightsPositionParameterStr, m_LightsPositionParameter );
  GetParameterBySemantic( LightsDirectionParameterStr, m_LightsDirectionParameter );
  GetParameterBySemantic( LightsAngleParameterStr, m_LightsAngleParameter );
  GetParameterBySemantic( LightsColorParameterStr, m_LightsColorParameter );
  GetParameterBySemantic( LightsFallOffParameterStr, m_LightsFallOffParameter );
  GetParameterBySemantic( LightsStartRangeAttenuationParameterStr,
                          m_LightsStartRangeAttenuationParameter );
  GetParameterBySemantic( LightsEndRangeAttenuationParameterStr,
                          m_LightsEndRangeAttenuationParameter );
  GetParameterBySemantic( CameraPositionParameterStr, m_CameraPositionParameter );
  GetParameterBySemantic( BonesParameterStr, m_BonesParameter );
  GetParameterBySemantic( TimeParameterStr, m_TimeParameter );
  GetParameterBySemantic( DebugColorStr, m_DebugColor );
  GetParameterBySemantic( UseDebugColorStr, m_UseDebugColor );
  GetParameterBySemantic( FogStartStr, m_FogStart );
  GetParameterBySemantic( FogEndStr, m_FogEnd );
  GetParameterBySemantic( FogExpStr, m_FogExp );
  GetParameterBySemantic( FogFunStr, m_FogFun );
  GetParameterBySemantic( TextureHeightStr, m_HeightTexture );
  GetParameterBySemantic( TextureWidthStr, m_WidthTexture );
  GetParameterBySemantic( UseShadowMaskTextureStr, m_UseShadowMaskTextureParameter );
  GetParameterBySemantic( UseShadowStaticStr, m_UseStaticShadowmapParameter );
  GetParameterBySemantic( UseShadowDynamicStr, m_UseDynamicShadowmapParameter );
  return true;
}

void CEffect::Unload()
{
  SetNullParameters();
  CHECKED_RELEASE( m_Effect );
}

bool CEffect::Load( CXMLTreeNode& EffectNode )
{
  m_FileName = EffectNode.GetPszProperty( "file", "no_file" );

  for ( int j = 0; j < EffectNode.GetNumChildren(); j++ )
  {
    CXMLTreeNode& l_CurrentSubNode = EffectNode( j );
    const std::string& l_TagName = l_CurrentSubNode.GetName();

    if ( l_TagName == "define" )
    {
      char* cstr_name = StringUtils::ToCharPtr( l_CurrentSubNode.GetPszProperty( "name", "no_name" ) );
      m_NamesMacrosChar.push_back( cstr_name );

      char* cstr_desc = StringUtils::ToCharPtr( l_CurrentSubNode.GetPszProperty( "description",
                        "no_description" ) );
      m_DescriptionsMacrosChar.push_back( cstr_desc );

      D3DXMACRO macro = { cstr_name, cstr_desc };
      m_Defines.push_back( macro );
    }
  }

  D3DXMACRO null = { NULL, NULL };
  m_Defines.push_back( null );

  return LoadEffect();
}

D3DXHANDLE CEffect::GetTechniqueByName( const std::string& TechniqueName )
{
  return ( m_Effect ) ? m_Effect->GetTechniqueByName( TechniqueName.c_str() ) : 0;
}
bool CEffect::SetLights( size_t NumOfLights )
{
  ResetLightsHandle();
  CLightManager* l_pLightManager = CLightManager::GetSingletonPtr();

  for ( size_t i = 0; i < NumOfLights; ++i )
  {
    CLight* l_pCurrentLight = l_pLightManager->GetLight( i );
    m_LightsEnabled[i] = ( BOOL )l_pCurrentLight == NULL ? 0 : 1;

    if ( l_pCurrentLight != NULL )
    {
      CLight::ELightType l_LightType = l_pCurrentLight->GetType();
      m_LightsType[i] = static_cast<int>( l_LightType );
      m_LightsStartRangeAttenuation[i] = l_pCurrentLight->GetStartRangeAttenuation();
      m_LightsEndRangeAttenuation[i] = l_pCurrentLight->GetEndRangeAttenuation();
      m_LightsPosition[i] = l_pCurrentLight->GetPosition();
      Math::CColor l_Color = l_pCurrentLight->GetColor();
      m_LightsColor[i] = Math::Vect3f( l_Color.GetRed() / 255.0f,
                                       l_Color.GetGreen() / 255.0f,
                                       l_Color.GetBlue() / 255.0f );

      if ( l_LightType == CLight::DIRECTIONAL )
      {
        CDirectionalLight* l_pDirectionalLight = static_cast<CDirectionalLight*>
            ( l_pCurrentLight );
        m_LightsDirection[i] = l_pDirectionalLight->GetDirection();
      }
      else if ( l_LightType == CLight::SPOT )
      {
        CSpotLight* l_SpotLight = static_cast<CSpotLight*>( l_pCurrentLight );
        m_LightsDirection[i] = l_SpotLight->GetDirection();
        m_LightsAngle[i] = l_SpotLight->GetAngle();
        m_LightsFallOff[i] = l_SpotLight->GetFallOff();
      }

      //Begin the render of the shadow
      l_pCurrentLight->BeginRenderEffectManagerShadowMap( this );
    }
  }

  return true;
}
bool CEffect::SetLight( size_t i_light )
{
  ResetLightsHandle();
  CLight* l_pCurrentLight = CLightManager::GetSingletonPtr()->GetLight( i_light );

  if ( !l_pCurrentLight )
  {
    LOG_ERROR_APPLICATION(
                                           "CEffect::SetLight->The light %d was not found in the light manager\n", ( int )i_light );
    return false;
  }

  m_LightsEnabled[0] = ( BOOL )l_pCurrentLight == NULL ? 0 : 1;
  CLight::ELightType l_LightType = l_pCurrentLight->GetType();
  m_LightsType[0] = static_cast<int>( l_LightType );
  m_LightsStartRangeAttenuation[0] = l_pCurrentLight->GetStartRangeAttenuation();
  m_LightsEndRangeAttenuation[0] = l_pCurrentLight->GetEndRangeAttenuation();
  m_LightsPosition[0] = l_pCurrentLight->GetPosition();
  Math::CColor l_Color = l_pCurrentLight->GetColor();
  m_LightsColor[0] = Math::Vect3f( l_Color.GetRed() / 255.0f,
                                   l_Color.GetGreen() / 255.0f, l_Color.GetBlue() / 255.0f );
  CDirectionalLight* l_pDirectionalLight = dynamic_cast<CDirectionalLight*>( l_pCurrentLight );

  if ( l_pDirectionalLight )
    m_LightsDirection[0] = l_pDirectionalLight->GetDirection();

  CSpotLight* l_SpotLight = dynamic_cast<CSpotLight*>( l_pCurrentLight );

  if ( l_SpotLight )
  {
    m_LightsAngle[0] = l_SpotLight->GetAngle();
    m_LightsFallOff[0] = l_SpotLight->GetFallOff();
  }

  //Begin the render of the shadow
  l_pCurrentLight->BeginRenderEffectManagerShadowMap( this );
  return true;
}
bool CEffect::SetCameraPosition( Math::Vect3f CameraPosition )
{
  float32 l_Camera[3];
  l_Camera[0] = CameraPosition.x;
  l_Camera[1] = CameraPosition.y;
  l_Camera[2] = CameraPosition.z;
  return ( m_Effect->SetFloatArray( m_CameraPositionParameter, l_Camera,
                                    3 ) == S_OK );
}
bool CEffect::SetWorldMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_WorldMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetViewMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_ViewMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetProjectionMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_ProjectionMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetInverseWorldMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_InverseWorldMatrixParameter,
                                &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetInverseViewMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_InverseViewMatrixParameter,
                                &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetInverseProjectionMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_InverseProjectionMatrixParameter,
                                &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetWorldViewMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_WorldViewMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_WorldViewProjectionMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetViewProjectionMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_ViewProjectionMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
bool CEffect::SetViewToLightMatrix( const Math::Mat44f& Matrix )
{
  return ( m_Effect->SetMatrix( m_ViewToLightProjectionMatrixParameter,
                                &Matrix.GetD3DXMatrix() ) == S_OK );
}
void CEffect::SetShadowMapParameters( bool UseShadowMaskTexture, bool
                                      UseStaticShadowmap, bool UseDynamicShadowmap )
{
  m_Effect->SetBool( m_UseShadowMaskTextureParameter, UseShadowMaskTexture ?
                     TRUE : FALSE );
  m_Effect->SetBool( m_UseStaticShadowmapParameter, UseStaticShadowmap ? TRUE :
                     FALSE );
  m_Effect->SetBool( m_UseDynamicShadowmapParameter, UseDynamicShadowmap ? TRUE
                     : FALSE );
}

void CEffect::SetUseDebugColor( bool aUse )
{
	m_Effect->SetBool( m_UseDebugColor, aUse ? TRUE : FALSE );
}
void CEffect::ResetLightsHandle()
{
  //Reset all the lights of the effect
  memset( m_LightsEnabled, 0, sizeof( BOOL ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsType, 0, sizeof( int32 ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsAngle, 0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsFallOff, 0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsStartRangeAttenuation, 0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsEndRangeAttenuation, 0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsPosition, 0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsDirection, 0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
  memset( m_LightsColor, 0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
}