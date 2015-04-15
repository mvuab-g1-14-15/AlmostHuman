#pragma once
#ifndef EFFECTS_DEFINES_H
#define EFFECTS_DEFINES_H

//-----------------------------------------------------------------------
#define MAX_LIGHTS_BY_SHADER 4
//-----------------------------------------------------------------------

// Matrix Handles -------------------------------------------------------
const char WorldMatrixParameterStr[] = "World";
const char ViewMatrixParameterStr[] = "View";
const char ProjectionMatrixParameterStr[] = "Projection";
const char InverseProjectionMatrixParameterStr[] = "ProjectionInverse";
const char InverseViewMatrixParameterStr[] = "ViewInverse";
const char InverseWorldMatrixParameterStr[] = "WorldInverse";
const char WorldViewMatrixParameterStr[] = "WorldView";
const char ViewProjectionMatrixParameterStr[] = "ViewProjection";
const char WorldViewProjectionMatrixParameterStr[] = "WorldViewProjection";
const char ViewToLightProjectionMatrixParameterStr[] = "ViewToLightProjection";

//-Lights Handles ---------------------------------------------------------
const char LightAmbientColorParameterStr[] = "AmbientLightColor";
const char LightEnabledParameterStr[] = "LightsEnabled";
const char LightsTypeParameterStr[] = "LightsTypes";
const char LightsPositionParameterStr[] = "LightsPositions";
const char LightsDirectionParameterStr[] = "LightsDirections";
const char LightsAngleParameterStr[] = "LightsAngles";
const char LightsFallOffParameterStr[] = "LightsFallOffs";
const char LightsColorParameterStr[] = "LightsColors";
const char LightsStartRangeAttenuationParameterStr[] = "LightsStartAngle";
const char LightsEndRangeAttenuationParameterStr[] = "LightsEndAngle";
const char CameraPositionParameterStr[] = "CameraPosition";
const char BonesParameterStr[] = "Bones";

//-----------------------------------------------------------------------
const char TimeParameterStr[]       = "Time";
const char DeltaTimeParameterStr[]  = "DeltaTime";

//-----------------------------------------------------------------------
const char DebugColorStr[]      = "DebugColor";
const char UseDebugColorStr[]   = "UseDebugColor";

//-Fog handles ----------------------------------------------------------
// Fog enum
enum EFogFunction
{
    eFogExponentianl = 0,
    eFogLineal,
    eFogGround,
    eFogFunctionCount
};

const char UseFogStr[]      = "UseFog";
const char FogStartStr[]    = "FogStart";
const char FogEndStr[]      = "FogEnd";
const char FogExpStr[]      = "FogExp";
const char FogFunStr[]      = "FogFun";
//-----------------------------------------------------------------------

//-Light handles --------------------------------------------------------

//-----------------------------------------------------------------------

//-Texture size handles -------------------------------------------------
const char TextureWidthStr[] = "TextureWidth";
const char TextureHeightStr[] = "TextureHeight";
//-----------------------------------------------------------------------

// Shadow handles -------------------------------------------------------
const char UseShadowMapTextureSizeStr[] = "g_ShadowMapTextureSize";
const char UseShadowMaskTextureStr[]    = "UseShadowMaskTexture";
const char UseShadowStaticStr[]         = "UseStaticShadowMap";
const char UseShadowDynamicStr[]        = "UseDynamicShadowMap";
//-----------------------------------------------------------------------
#endif //EFFECTS_DEFINES_H