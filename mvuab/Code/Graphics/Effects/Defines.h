#pragma once
#ifndef EFFECTS_DEFINES_H
#define EFFECTS_DEFINES_H

enum EMatrixFlags
{
    eWorldMatrix                 = 1 << 0,
    eWVMatrix                    = 1 << 1,
    eWVPMatrix                   = 1 << 2,
    eInverseWorld                = 1 << 3,
};

enum ELightsFlags
{
    eUseLights                   = 1 << 0,
    eViewToLightProjectionMatrix = 1 << 1
};

#define DECLARE_EFFECT_PARAMETER( semantic ) D3DXHANDLE m_##semantic;
#define CTOR_EFFECT_PARAMETER( semantic ) m_##semantic( 0 )
#define RESET_EFFECT_PARAMETER( semantic ) m_##semantic = 0;
#define LINK_EFFECT_PARAMETER( parameter ) GetParameterBySemantic( #parameter, m_##parameter );

#define SET_FLOAT_PARAMETER( parameter, value ) m_Effect->SetFloat( m_##parameter, value )
#define SET_INT_PARAMETER( parameter, value ) m_Effect->SetInt( m_##parameter, value )
#define SET_BOOL_PARAMETER( parameter, value ) m_Effect->SetBool( m_##parameter, value ? TRUE : FALSE )
#define SET_MATRIX_PARAMETER( parameter, value ) m_Effect->SetMatrix( m_##parameter, &((Matrix).GetD3DXMatrix()) );

//-----------------------------------------------------------------------
#define MAX_LIGHTS_BY_SHADER 4
//-----------------------------------------------------------------------

// Matrix Handles -------------------------------------------------------
const char ViewToLightProjectionMatrixParameterStr[] = "ViewToLightProjection";

//-Lights Handles ---------------------------------------------------------
const char LightEnabledParameterStr[]                 = "LightsEnabled";
const char LightsTypeParameterStr[]                   = "LightsTypes";
const char LightsPositionParameterStr[]               = "LightsPositions";
const char LightsDirectionParameterStr[]              = "LightsDirections";
const char LightsAngleParameterStr[]                  = "LightsAngles";
const char LightsFallOffParameterStr[]                = "LightsFallOffs";
const char LightsColorParameterStr[]                  = "LightsColors";
const char LightsIntensityParameterStr[]              = "LightsIntensity";
const char LightsStartRangeAttenuationParameterStr[]  = "LightsStartAngle";
const char LightsEndRangeAttenuationParameterStr[]    = "LightsEndAngle";
const char BonesParameterStr[] = "Bones";
const char LightProbesParameterStr[] = "LightProbes";

//-Debug Handles----------------------------------------------------------
const char DebugColorStr[]      = "DebugColor";
const char UseDebugColorStr[]   = "UseDebugColor";

//-Fog handles ----------------------------------------------------------
const char UseFogStr[]      = "UseFog";
const char FogStartStr[]    = "FogStart";
const char FogEndStr[]      = "FogEnd";
const char FogExpStr[]      = "FogExp";
const char FogFunStr[]      = "FogFun";

// Fog enum
enum EFogFunction
{
    eFogExponentianl = 0,
    eFogLineal,
    eFogGround,
    eFogFunctionCount
};

//-Size handles ----------------------------------------------------------
const char Size[]      = "Size";

//-Texture size handles -------------------------------------------------
const char TextureWidthStr[] = "TextureWidth";
const char TextureHeightStr[] = "TextureHeight";
//-----------------------------------------------------------------------

//-Window size handles -------------------------------------------------
const char WindowWidthStr[] = "WindowWidth";
const char WindowHeightStr[] = "WindowHeight";
//-----------------------------------------------------------------------

// Shadow handles -------------------------------------------------------
const char UseShadowMapTextureSizeStr[] = "g_ShadowMapTextureSize";
const char UseShadowMaskTextureStr[]    = "UseShadowMaskTexture";
const char UseShadowStaticStr[]         = "UseStaticShadowMap";
const char UseShadowDynamicStr[]        = "UseDynamicShadowMap";
const char ShadowMapTextureSizeStr[]    = "ShadowMapTextureSize";
//-----------------------------------------------------------------------


#endif //EFFECTS_DEFINES_H