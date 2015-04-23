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
const char LightAmbientColorParameterStr[]            = "AmbientLightColor";
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

//-Time Handles-----------------------------------------------------------
const char CameraPositionParameterStr[] = "CameraPosition";
const char CameraUpParameterStr[]       = "CameraUp";
const char CameraLeftParameterStr[]     = "CameraLeft";
const char CameraLookAtParameterStr[]   = "CameraLookAt";

//-Time Handles-----------------------------------------------------------
const char TimeParameterStr[]       = "Time";
const char DeltaTimeParameterStr[]  = "DeltaTime";

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