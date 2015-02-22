#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include <d3dx9.h>
#include <string>
#include "Math\Matrix44.h"
#include "Effects\Defines.h"
#include "Utils\Name.h"
#include "Utils\Defines.h"

class CLight;

class CEffect: public CName
{
public:
  CEffect();
  CEffect( const std::string& EffectName );
  ~CEffect();
  bool SetLights( size_t NumOfLights );
  bool SetLight( size_t i_light );
  bool SetCameraPosition( Math::Vect3f CameraPosition );
  bool SetWorldMatrix( const Math::Mat44f& Matrix, bool Inverse = false );
  bool SetViewMatrix( const Math::Mat44f& Matrix, bool Inverse = false );
  bool SetProjectionMatrix( const Math::Mat44f& Matrix, bool Inverse = false );
  bool SetWorldViewMatrix( const Math::Mat44f& Matrix );
  bool SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix );
  bool SetViewProjectionMatrix( const Math::Mat44f& Matrix );
  bool SetViewToLightMatrix( const Math::Mat44f& Matrix );

  bool Load( const std::string& FileName );
  bool Reload();
  //DirectX Methods Interface
  D3DXHANDLE GetTechniqueByName( const std::string& TechniqueName );

  GET_SET( D3DXHANDLE, WorldMatrixParameter );
  GET_SET( D3DXHANDLE, ViewMatrixParameter );
  GET_SET( D3DXHANDLE, ProjectionMatrixParameter );
  GET_SET( D3DXHANDLE, WorldViewMatrixParameter );
  GET_SET( D3DXHANDLE, ViewProjectionMatrixParameter );
  GET_SET( D3DXHANDLE, WorldViewProjectionMatrixParameter );
  GET_SET( D3DXHANDLE, ViewToLightProjectionMatrixParameter );
  GET_SET( D3DXHANDLE, InverseProjectionMatrixParameter );
  GET_SET( D3DXHANDLE, InverseViewMatrixParameter );
  GET_SET( D3DXHANDLE, InverseWorldMatrixParameter );

  GET_SET( D3DXHANDLE, LightEnabledParameter );
  GET_SET( D3DXHANDLE, LightsTypeParameter );
  GET_SET( D3DXHANDLE, LightsPositionParameter );
  GET_SET( D3DXHANDLE, LightsDirectionParameter );
  GET_SET( D3DXHANDLE, LightsAngleParameter );
  GET_SET( D3DXHANDLE, LightsColorParameter );
  GET_SET( D3DXHANDLE, LightsFallOffParameter );
  GET_SET( D3DXHANDLE, LightsStartRangeAttenuationParameter );
  GET_SET( D3DXHANDLE, LightsEndRangeAttenuationParameter );

  GET_SET( D3DXHANDLE, CameraPositionParameter );
  GET_SET( D3DXHANDLE, BonesParameter );
  GET_SET( D3DXHANDLE, TimeParameter );

  GET_SET( D3DXHANDLE, DebugColor );

  GET_SET( D3DXHANDLE, SceneTextureParameter );
  GET_SET( D3DXHANDLE, BloomThresholdParameter );
  GET_SET( D3DXHANDLE, SampleOffsetsParameter );
  GET_SET( D3DXHANDLE, SampleWeightsParameter );
  GET_SET( D3DXHANDLE, GaussianBlurTextureParameter );
  GET_SET( D3DXHANDLE, BloomIntensityParameter );
  GET_SET( D3DXHANDLE, BaseIntensityParameter );
  GET_SET( D3DXHANDLE, BloomSaturationParameter );
  GET_SET( D3DXHANDLE, BaseSaturationParameter );
  GET_SET( D3DXHANDLE, PostBloomTextureParameter );

  GET_SET_REF( std::string, FileName );
  GET_SET( LPD3DXEFFECT, Effect );

  const BOOL* GetLightsEnabled()
  {
    return m_LightsEnabled;
  }
  const int32* GetLightsType()
  {
    return m_LightsType;
  }
  const float32* GetLightsAngle()
  {
    return m_LightsAngle;
  }
  const float32* GetLightsFallOff()
  {
    return m_LightsFallOff;
  }
  const float32* GetLightsStartRangeAttenuation()
  {
    return m_LightsStartRangeAttenuation;
  }
  const float32* GetLightsEndRangeAttenuation()
  {
    return m_LightsEndRangeAttenuation;
  }
  const Math::Vect3f* GetLightsPosition()
  {
    return m_LightsPosition;
  }
  const Math::Vect3f* GetLightsDirection()
  {
    return m_LightsDirection;
  }
  const Math::Vect3f* GetLightsColor()
  {
    return m_LightsColor;
  }

  void SetShadowMapParameters( bool UseShadowMaskTexture, bool UseStaticShadowmap,
                               bool UseDynamicShadowmap );

private: // Members
  std::string m_FileName;
  LPD3DXEFFECT m_Effect;
  BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
  int32 m_LightsType[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsAngle[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];
  D3DXHANDLE m_WorldMatrixParameter, m_ViewMatrixParameter,
             m_ProjectionMatrixParameter, m_InverseProjectionMatrixParameter,
             m_InverseViewMatrixParameter, m_InverseWorldMatrixParameter;
  D3DXHANDLE m_WorldViewMatrixParameter, m_ViewProjectionMatrixParameter,
             m_WorldViewProjectionMatrixParameter;
  D3DXHANDLE m_ViewToLightProjectionMatrixParameter;
  D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter,
             m_LightsPositionParameter, m_LightsDirectionParameter, m_LightsAngleParameter,
             m_LightsColorParameter;
  D3DXHANDLE m_LightsFallOffParameter, m_LightsStartRangeAttenuationParameter,
             m_LightsEndRangeAttenuationParameter;
  D3DXHANDLE m_CameraPositionParameter;
  D3DXHANDLE m_BonesParameter;
  D3DXHANDLE m_TimeParameter;
  D3DXHANDLE m_DebugColor;
  D3DXHANDLE m_UseShadowMaskTextureParameter;
  D3DXHANDLE m_UseStaticShadowmapParameter;
  D3DXHANDLE m_UseDynamicShadowmapParameter;

  D3DXHANDLE m_SceneTextureParameter;
  D3DXHANDLE m_BloomThresholdParameter;
  D3DXHANDLE m_SampleOffsetsParameter;
  D3DXHANDLE m_SampleWeightsParameter;
  D3DXHANDLE m_GaussianBlurTextureParameter;
  D3DXHANDLE m_BloomIntensityParameter;
  D3DXHANDLE m_BaseIntensityParameter;
  D3DXHANDLE m_BloomSaturationParameter;
  D3DXHANDLE m_BaseSaturationParameter;
  D3DXHANDLE m_PostBloomTextureParameter;

  /*
  SceneTextureParameter );
  BloomThresholdParameter );
  SampleOffsetsParameter );
  SampleWeightsParameter );
  GaussianBlurTextureParameter );
  BloomIntensityParameter );
  BaseIntensityParameter );
  BloomSaturationParameter );
  BaseSaturationParameter );
  PostBloomTextureParameter );
  */


private: // Methods
  void  SetNullParameters();
  void GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle );
  void GetParameterBySemantic( const std::string& SemanticName,
                               D3DXHANDLE& a_Handle );
  bool LoadEffect();
  void Unload();
};
#endif // EFFECT_H