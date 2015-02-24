#pragma once
#ifndef EFFECTS_DEFINES_H
#define EFFECTS_DEFINES_H

#define MAX_LIGHTS_BY_SHADER 4

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
const char TimeParameterStr[] = "DeltaTime";
const char DebugColorStr[] = "DebugColor";

const char FogStartStr[] = "FogStart";
const char FogEndStr[] = "FogEnd";

const char FogExpStr[] = "FogExp";
const char FogFunStr[] = "FogFun";
const char WeightsStr[] = "Weight";
const char OffsetsStr[] = "Offset";

#endif //EFFECTS_DEFINES_H