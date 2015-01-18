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
const char LightEnabledParameterStr[] = "LightEnabled";
const char LightsTypeParameterStr[] = "LightsTypes";
const char LightsPositionParameterStr[] = "LightsPosition";
const char LightsDirectionParameterStr[] = "LightsDirection";
const char LightsAngleParameterStr[] = "LightsAngle";
const char LightsFallOffParameterStr[] = "LightsFallOff";
const char LightsColorParameterStr[] = "LightsColor";
const char LightsStartRangeAttenuationParameterStr[] = "LightsStartAngle";
const char LightsEndRangeAttenuationParameterStr[] = "LightsEndAngle";
const char CameraPositionParameterStr[] = "CameraPosition";
const char BonesParameterStr[] = "Bones";
const char TimeParameterStr[] = "DeltaTime";

#endif //EFFECTS_DEFINES_H