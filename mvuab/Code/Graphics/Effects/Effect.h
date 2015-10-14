#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "Effects\Defines.h"
#include "Math\Matrix44.h"
#include "Effects\Defines.h"
#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

class CLight;

class CEffect: public CName
{
public:
  CEffect( const std::string& EffectName );
  virtual ~CEffect();

  void LinkSemantics();
  bool SetLights( size_t NumOfLights );
  bool SetLight( size_t i_light );

  bool SetViewToLightMatrix( const Math::Mat44f& Matrix );

  bool Load  ( const std::string& aFileName, const std::vector<SDefines>& aDefines );
  void Unload();

  bool SetWorldMatrix               ( const Math::Mat44f& Matrix );
  bool SetInverseWorldMatrix        ( const Math::Mat44f& Matrix );
  bool SetWorldViewMatrix           ( const Math::Mat44f& Matrix );
  bool SetWorldViewProjectionMatrix ( const Math::Mat44f& Matrix );
  bool SetCameraPosition            ( const Math::Vect3f &CameraPosition );
  bool SetViewMatrix                ( const Math::Mat44f& Matrix );
  bool SetProjectionMatrix          ( const Math::Mat44f& Matrix );
  bool SetInverseViewMatrix         ( const Math::Mat44f& Matrix );
  bool SetInverseProjectionMatrix   ( const Math::Mat44f& Matrix );
  bool SetViewProjectionMatrix      ( const Math::Mat44f& Matrix );
  bool SetAmbientLightColor         ( const Math::Vect3f &aAmbienLightColor );
  bool SetFBSize                    ( const Math::Vect2u aSize );
  bool SetDeltaTime                 ( const float dt );
  bool SetPercentageTextures        ( float aPercentageTexture1, float aPercentageTexture2 );

  //DirectX Methods Interface
  D3DXHANDLE GetTechniqueByName( const std::string& TechniqueName );

  GET_SET( D3DXHANDLE, ViewToLightProjectionMatrixParameter );

  GET_SET( D3DXHANDLE, LightEnabledParameter );
  GET_SET( D3DXHANDLE, LightsTypeParameter );
  GET_SET( D3DXHANDLE, LightsPositionParameter );
  GET_SET( D3DXHANDLE, LightsDirectionParameter );
  GET_SET( D3DXHANDLE, LightsAngleParameter );
  GET_SET( D3DXHANDLE, LightsColorParameter );
  GET_SET( D3DXHANDLE, LightsFallOffParameter );
  GET_SET( D3DXHANDLE, LightsStartRangeAttenuationParameter );
  GET_SET( D3DXHANDLE, LightsEndRangeAttenuationParameter );
  GET_SET( D3DXHANDLE, BonesParameter );
  GET_SET( D3DXHANDLE, LightProbesParameter );
  GET_SET( D3DXHANDLE, DebugColor );
  GET_SET( D3DXHANDLE, UseDebugColor );
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

  void SetFlipUVs( bool Horizontal, bool aVertical );
  void SetShadowMapParameters( bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap );
  void SetDebugColor( bool aUse, const Math::CColor aColor );
  void SetSize( float aSize );
  void SetAlpha( float aAlpha );
  void SetAngle( float aAngle );
  void SetZBlur(float FocalStart, float FocalEnd, float Constant, float BlurEnd);
  void SetScatteredLight(float l_Decay, float l_Esposure, float l_Weight, float l_Density, int l_Samples, Math::Vect3f &l_Pos, Math::Vect2i &l_TexSize);
  void SetDirection( const Math::Vect3f& aDirection );

protected:
  LPD3DXEFFECT m_Effect;
  virtual void  SetNullParameters();
  void GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle );
  void GetParameterBySemantic( const std::string& SemanticName, D3DXHANDLE& a_Handle );

private: // Members
  std::string m_FileName;
  BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
  int32 m_LightsType[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsAngle[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  float32 m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
  Math::Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];

  DECLARE_EFFECT_PARAMETER( PercentageTexture1 );
  DECLARE_EFFECT_PARAMETER( PercentageTexture2 );

  DECLARE_EFFECT_PARAMETER( Direction );
  DECLARE_EFFECT_PARAMETER( Size );
  DECLARE_EFFECT_PARAMETER( Angle );
  DECLARE_EFFECT_PARAMETER( Alpha );
  DECLARE_EFFECT_PARAMETER( Color );
  DECLARE_EFFECT_PARAMETER( Time );

  DECLARE_EFFECT_PARAMETER( InverseWorldMatrix );
  DECLARE_EFFECT_PARAMETER( WorldMatrix );
  DECLARE_EFFECT_PARAMETER( WVMatrix );
  DECLARE_EFFECT_PARAMETER( WVPMatrix );
  DECLARE_EFFECT_PARAMETER( ViewMatrix );
  DECLARE_EFFECT_PARAMETER( ProjectionMatrix );
  DECLARE_EFFECT_PARAMETER( InverseViewMatrix );
  DECLARE_EFFECT_PARAMETER( InverseProjectionMatrix );
  DECLARE_EFFECT_PARAMETER( VPMatrix );
  DECLARE_EFFECT_PARAMETER( CameraPosition );
  DECLARE_EFFECT_PARAMETER( AmbientLight );
  DECLARE_EFFECT_PARAMETER( FBWidth );
  DECLARE_EFFECT_PARAMETER( FBHeight );
  DECLARE_EFFECT_PARAMETER( DeltaTime );
  DECLARE_EFFECT_PARAMETER( FlipUVHorizontal );
  DECLARE_EFFECT_PARAMETER( FlipUVVertical   );

  D3DXHANDLE m_ViewToLightProjectionMatrixParameter;

  D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter,
             m_LightsPositionParameter, m_LightsDirectionParameter, m_LightsAngleParameter,
             m_LightsColorParameter;

  D3DXHANDLE m_LightsFallOffParameter, m_LightsStartRangeAttenuationParameter,
             m_LightsEndRangeAttenuationParameter;

  D3DXHANDLE m_BonesParameter;

  D3DXHANDLE m_LightProbesParameter;

  D3DXHANDLE m_DebugColor;
  D3DXHANDLE m_UseDebugColor;

  //
  // ZBlur
  //
  D3DXHANDLE m_ZBlurFocalStart;
  D3DXHANDLE m_ZBlurFocalEnd;
  D3DXHANDLE m_ZBlurConstant;
  D3DXHANDLE m_ZBlurEnd;

  //
  // Scattered Ligh
  //
  D3DXHANDLE m_SLDecai;
  D3DXHANDLE m_SLExposure;
  D3DXHANDLE m_SLDensity;
  D3DXHANDLE m_SLWeight;
  D3DXHANDLE m_SLSamples;
  D3DXHANDLE m_SLLightPos;

  //
  // Shadow map handles
  //
  D3DXHANDLE m_ShadowMapTextureSizeParameter;
  D3DXHANDLE m_UseShadowMaskTextureParameter;
  D3DXHANDLE m_UseStaticShadowmapParameter;
  D3DXHANDLE m_UseDynamicShadowmapParameter;

private: // Methods
  void ResetLightsHandle();
};
#endif // EFFECT_H