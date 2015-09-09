#include "Effect.h"

#include "EffectManager.h"
#include "EffectPool.h"
#include "EffectTechnique.h"
#include "GraphicsManager.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightManager.h"
#include "Lights/SpotLight.h"
#include "Logger\Logger.h"
#include "Utils/BaseUtils.h"
#include "Utils/StringUtils.h"
#include "EngineManagers.h"
#include "Effects\Defines.h"

CEffect::CEffect( const std::string& EffectName )
    : CName( EffectName ),
      m_FileName( "" ),
      m_Effect( 0 ),
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
      m_BonesParameter( 0 ),
	  m_LightProbesParameter( 0 ),

      // Debug data
      m_UseDebugColor( 0 ),
      m_DebugColor( 0 ),

      // Fog
      m_FogStart( 0 ),
      m_FogEnd( 0 ),
      m_FogExp( 0 ),
      m_FogFun( 0 ),

      // Texture
      m_HeightTexture( 0 ),
      m_WidthTexture( 0 )

    , CTOR_EFFECT_PARAMETER( Size )
    , CTOR_EFFECT_PARAMETER( Angle )
    , CTOR_EFFECT_PARAMETER( Alpha )
    , CTOR_EFFECT_PARAMETER( Color )
    , CTOR_EFFECT_PARAMETER( InverseWorldMatrix )
    , CTOR_EFFECT_PARAMETER( WorldMatrix )
    , CTOR_EFFECT_PARAMETER( WVMatrix )
    , CTOR_EFFECT_PARAMETER( WVPMatrix )
	, CTOR_EFFECT_PARAMETER( Direction )
{
    ResetLightsHandle();
}

CEffect::~CEffect()
{
    m_Effect->End();
    CHECKED_RELEASE( m_Effect );

    for ( size_t i = 0; i < m_NamesMacrosChar.size(); ++i )
    {
        delete m_NamesMacrosChar[i];
    }

    m_NamesMacrosChar.clear();

    for ( size_t i = 0; i < m_DescriptionsMacrosChar.size(); ++i )
    {
        delete m_DescriptionsMacrosChar[i];
    }

    m_DescriptionsMacrosChar.clear();
    m_Defines.clear();
}

void CEffect::SetNullParameters()
{
    // Reset the parameters
    RESET_EFFECT_PARAMETER( InverseWorldMatrix );
    RESET_EFFECT_PARAMETER( WorldMatrix );
    RESET_EFFECT_PARAMETER( WVMatrix );
    RESET_EFFECT_PARAMETER( WVPMatrix );
	RESET_EFFECT_PARAMETER( Direction );

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

    m_BonesParameter = 0;

	m_LightProbesParameter = 0;

    // Fog
    m_FogStart = 0;
    m_FogEnd = 0;
    m_FogExp = 0;
    m_FogFun = 0;
    // Texture
    m_HeightTexture = 0;
    m_WidthTexture = 0;

    RESET_EFFECT_PARAMETER( Size )
    RESET_EFFECT_PARAMETER( Angle )

    RESET_EFFECT_PARAMETER( Alpha )
    RESET_EFFECT_PARAMETER( Color )

    ResetLightsHandle();
}

void CEffect::LinkSemantics()
{
    // Get the references to the handlers inside the effect
    LINK_EFFECT_PARAMETER( Size  );
    LINK_EFFECT_PARAMETER( Angle );
    LINK_EFFECT_PARAMETER( Alpha );
    LINK_EFFECT_PARAMETER( Color );
	LINK_EFFECT_PARAMETER( Direction );

    LINK_EFFECT_PARAMETER( InverseWorldMatrix );
    LINK_EFFECT_PARAMETER( WorldMatrix );
    LINK_EFFECT_PARAMETER( WVMatrix );
    LINK_EFFECT_PARAMETER( WVPMatrix );

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

    GetParameterBySemantic( BonesParameterStr, m_BonesParameter );

	GetParameterBySemantic( LightProbesParameterStr, m_LightProbesParameter );

    GetParameterBySemantic( DebugColorStr, m_DebugColor );
    GetParameterBySemantic( UseDebugColorStr, m_UseDebugColor );

    GetParameterBySemantic( FogStartStr, m_FogStart );
    GetParameterBySemantic( FogEndStr, m_FogEnd );
    GetParameterBySemantic( FogExpStr, m_FogExp );
    GetParameterBySemantic( FogFunStr, m_FogFun );

    GetParameterBySemantic( TextureHeightStr, m_HeightTexture );
    GetParameterBySemantic( TextureWidthStr, m_WidthTexture );

    GetParameterBySemantic( WindowHeightStr, m_HeightWindow );
    GetParameterBySemantic( WindowWidthStr, m_WidthWindow );

    GetParameterBySemantic( ShadowMapTextureSizeStr, m_ShadowMapTextureSizeParameter );
    GetParameterBySemantic( UseShadowMaskTextureStr, m_UseShadowMaskTextureParameter );
    GetParameterBySemantic( UseShadowStaticStr, m_UseStaticShadowmapParameter );
    GetParameterBySemantic( UseShadowDynamicStr, m_UseDynamicShadowmapParameter );
    GetParameterBySemantic( UseFogStr, m_UseFog );
}

void CEffect::GetParameterBySemantic( const std::string& SemanticName, D3DXHANDLE& a_Handle )
{
    a_Handle = m_Effect->GetParameterBySemantic( 0, SemanticName.c_str() );
    ASSERT( a_Handle, "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName.c_str(),
            m_FileName.c_str() );
}

void CEffect::GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle )
{
    a_Handle = m_Effect->GetParameterBySemantic( 0, SemanticName );
    ASSERT( a_Handle, "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName,
            m_FileName.c_str() );
}

bool CEffect::LoadEffect( CEffectPool* aEffectPool )
{
    bool lOk(true);
    // Obtain the device from the graphics manager and load the effect
    LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    DWORD dwShaderFlags = 0;
    dwShaderFlags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
    LPD3DXBUFFER l_ErrorBuffer = 0;
    HRESULT l_HR = D3DXCreateEffectFromFile(
                       l_Device,
                       m_FileName.c_str(),
                       &m_Defines[0],
                       0, // LPD3DXINCLUDE pInclude,
                       dwShaderFlags,
                       (aEffectPool) ? aEffectPool->GetD3DEffectPool() : 0,
                       &m_Effect,
                       &l_ErrorBuffer );

    std::string lErrorMsg;
    if ( l_ErrorBuffer )
    {
        lOk = false;
        StringUtils::Format( lErrorMsg, "Error creating effect '%s': \n\n %s \n", m_FileName.c_str(),
                             l_ErrorBuffer->GetBufferPointer() );

        ASSERT( l_HR == S_OK, "Error creating effect '%s': \n % s", m_FileName.c_str(), l_ErrorBuffer->GetBufferPointer() );
        CHECKED_RELEASE( l_ErrorBuffer );
    }

    ASSERT( l_HR == S_OK, "%s %s", m_FileName.c_str(), lErrorMsg.c_str() );

    if( !lOk )
    {
        CHECKED_RELEASE( m_Effect );
    }
    else
    {
        LinkSemantics();
    }

    return lOk;
}

bool CEffect::Reload(CEffectPool* aEffectPool)
{
    Unload();
    return LoadEffect(aEffectPool);
}

void CEffect::Unload()
{
    SetNullParameters();
    CHECKED_RELEASE( m_Effect );
}

bool CEffect::Load( CXMLTreeNode& EffectNode, CEffectPool* aEffectPool )
{
    m_FileName = EffectNode.GetAttribute<std::string>( "file", "no_file" );

    for ( uint32 j = 0, lCount = EffectNode.GetNumChildren(); j < lCount; ++j )
    {
        const CXMLTreeNode& l_CurrentSubNode = EffectNode( j );
        const std::string& l_TagName = l_CurrentSubNode.GetName();

        if ( l_TagName == "define" )
        {
            char* cstr_name = StringUtils::ToCharPtr( l_CurrentSubNode.GetAttribute<std::string>( "name", "no_name" ) );
            m_NamesMacrosChar.push_back( cstr_name );
            char* cstr_desc = StringUtils::ToCharPtr( l_CurrentSubNode.GetAttribute<std::string>( "description",
                              "no_description" ) );
            m_DescriptionsMacrosChar.push_back( cstr_desc );
            D3DXMACRO macro = { cstr_name, cstr_desc };
            m_Defines.push_back( macro );
        }
    }

    D3DXMACRO null = { NULL, NULL };
    m_Defines.push_back( null );
    return LoadEffect(aEffectPool);
}

D3DXHANDLE CEffect::GetTechniqueByName( const std::string& TechniqueName )
{
    return ( m_Effect ) ? m_Effect->GetTechniqueByName( TechniqueName.c_str() ) : 0;
}

bool CEffect::SetLights( size_t NumOfLights )
{
    ResetLightsHandle();
    CLightManager* l_pLightManager = LightMInstance;

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
            m_LightsColor[i] = Math::Vect3f( l_Color.GetRed() ,
                                             l_Color.GetGreen() ,
                                             l_Color.GetBlue() );

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
    CLight* lLight = LightMInstance->GetLight( i_light );

    if ( lLight )
    {
        m_LightsEnabled[0] = ( BOOL )lLight == NULL ? 0 : 1;
        m_LightsType[0] = static_cast<int>( lLight->GetType() );
        m_LightsStartRangeAttenuation[0] = lLight->GetStartRangeAttenuation();
        m_LightsEndRangeAttenuation[0] = lLight->GetEndRangeAttenuation();
        m_LightsPosition[0] = lLight->GetPosition();
        Math::CColor l_Color = lLight->GetColor();
        m_LightsColor[0] = Math::Vect3f( l_Color.GetRed(), l_Color.GetGreen(), l_Color.GetBlue() );
        CDirectionalLight* l_pDirectionalLight = dynamic_cast<CDirectionalLight*>( lLight );
        if ( l_pDirectionalLight )
        {
            m_LightsDirection[0] = l_pDirectionalLight->GetDirection();

            CSpotLight* lSpotLight = dynamic_cast<CSpotLight*>( lLight );
            if ( lSpotLight )
            {
                m_LightsAngle[0] = lSpotLight->GetAngle();
                m_LightsFallOff[0] = lSpotLight->GetFallOff();
            }
        }

        //Begin the render of the shadow
        lLight->BeginRenderEffectManagerShadowMap( this );
    }
    return true;
}

bool CEffect::SetViewToLightMatrix( const Math::Mat44f& Matrix )
{
    return ( m_Effect->SetMatrix( m_ViewToLightProjectionMatrixParameter,
                                  &Matrix.GetD3DXMatrix() ) == S_OK );
}
void CEffect::SetShadowMapParameters
(
    bool UseShadowMaskTexture,
    bool UseStaticShadowmap,
    bool UseDynamicShadowmap
)
{
    m_Effect->SetBool( m_UseShadowMaskTextureParameter, UseShadowMaskTexture ?
                       TRUE : FALSE );
    m_Effect->SetBool( m_UseStaticShadowmapParameter, UseStaticShadowmap ? TRUE :
                       FALSE );
    m_Effect->SetBool( m_UseDynamicShadowmapParameter, UseDynamicShadowmap ? TRUE
                       : FALSE );
}

void CEffect::SetDebugColor( bool aUse, const Math::CColor aColor )
{
    m_Effect->SetBool( m_UseDebugColor, aUse ? TRUE : FALSE );

    if ( aUse )
    {
        float32 l_DebugColor[4];
        l_DebugColor[0] = aColor.GetRed();
        l_DebugColor[1] = aColor.GetGreen();
        l_DebugColor[2] = aColor.GetBlue();
        l_DebugColor[3] = aColor.GetAlpha();
        m_Effect->SetFloatArray( m_DebugColor, l_DebugColor, sizeof( float ) * 4 );
    }
}

void CEffect::SetFog
(
    bool aUseFog,
    float32 aFogStart,
    float32 aFogEnd,
    float32 aFogExponent,
    EFogFunction aFogFun
)
{
    m_Effect->SetBool( m_UseFog,     aUseFog ? TRUE : FALSE );
    m_Effect->SetInt( m_FogFun, ( int ) aFogFun );
    m_Effect->SetFloat( m_FogStart,   aFogStart );
    m_Effect->SetFloat( m_FogEnd,     aFogEnd );
    m_Effect->SetFloat( m_FogExp,     aFogExponent );
}

void CEffect::ResetLightsHandle()
{
    //Reset all the lights of the effect
    memset( m_LightsEnabled,                  0, sizeof( BOOL ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsType,                     0, sizeof( int32 ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsAngle,                    0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsFallOff,                  0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsStartRangeAttenuation,    0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsEndRangeAttenuation,      0, sizeof( float32 ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsPosition,                 0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsDirection,                0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
    memset( m_LightsColor,                    0, sizeof( Math::Vect3f ) * MAX_LIGHTS_BY_SHADER );
}

void CEffect::SetSize( float aSize )
{
    HRESULT lRes = SET_FLOAT_PARAMETER( Size, aSize );
    ASSERT( lRes == S_OK, "Error setting size");
}

void CEffect::SetAlpha( float aAlpha )
{
    HRESULT lRes = SET_FLOAT_PARAMETER( Alpha, aAlpha );
    ASSERT( lRes == S_OK, "Error setting alpha");
}

void CEffect::SetDirection( const Math::Vect3f& aDirection )
{
	float32 l_Direction[3];
    l_Direction[0] = aDirection.x;
    l_Direction[1] = aDirection.y;
    l_Direction[2] = aDirection.z;
	HRESULT lRes = m_Effect->SetFloatArray( m_Direction, l_Direction, sizeof( float ) * 3 );
    ASSERT( lRes == S_OK, "Error setting direction");
}

void CEffect::SetAngle( float aAngle )
{
    HRESULT lRes = SET_FLOAT_PARAMETER( Angle, aAngle );
    ASSERT( lRes == S_OK, "Error setting size");
}

bool CEffect::SetWorldMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( WorldMatrix, Matrix );
}

bool CEffect::SetInverseWorldMatrix( const Math::Mat44f& Matrix )
{
    return ( m_Effect->SetMatrix( m_InverseWorldMatrix, &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
}

bool CEffect::SetWorldViewMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( WVMatrix, Matrix );
}

bool CEffect::SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( WVPMatrix, Matrix );
}