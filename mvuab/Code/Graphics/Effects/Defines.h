#pragma once
#ifndef EFFECTS_DEFINES_H
#define EFFECTS_DEFINES_H

#define MAX_LIGHTS_BY_SHADER 1

const char WorldMatrixParameterStr[] = "WORLD";
const char ViewMatrixParameterStr[] = "VIEW";
const char ProjectionMatrixParameterStr[] = "PROJECTION";
const char InverseProjectionMatrixParameterStr[] = "INVERSE_PROJECTION";
const char InverseViewMatrixParameterStr[] = "INVERSE_VIEW";
const char InverseWorldMatrixParameterStr[] = "INVERSE_WORLD";
const char WorldViewMatrixParameterStr[] = "WORLD_VIEW";
const char ViewProjectionMatrixParameterStr[] = "VIEW_PROJECTION";
const char WorldViewProjectionMatrixParameterStr[] = "WORLD_VIEW_PROJECTION";
const char ViewToLightProjectionMatrixParameterStr[] = "VIEW_TO_LIGHT_PROJECTION";
const char LightEnabledParameterStr[] = "LIGHT_ENABLED";
const char LightsTypeParameterStr[] = "LIGHTS_TYPES";
const char LightsPositionParameterStr[] = "LIGHTS_POSITION";
const char LightsDirectionParameterStr[] = "LIGHTS_DIRECTION";
const char LightsAngleParameterStr[] = "LIGHTS_ANGLE";
const char LightsFallOffParameterStr[] = "LIGHTS_FALL_OFF";
const char LightsColorParameterStr[] = "LIGHTS_COLOR";
const char LightsStartRangeAttenuationParameterStr[] = "LIGHTS_START_ANGLE";
const char LightsEndRangeAttenuationParameterStr[] = "LIGHTS_END_ANGLE";
const char CameraPositionParameterStr[] = "EYE";
const char BonesParameterStr[] = "BONES";
const char TimeParameterStr[] = "TIME";

#endif //EFFECTS_DEFINES_H