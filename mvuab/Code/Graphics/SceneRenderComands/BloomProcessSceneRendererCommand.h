#pragma once

#ifndef _BLOOM_PROCESS_SCENE_RENDERER_PROCESS_H
#define _BLOOM_PROCESS_SCENE_RENDERER_PROCESS_H

#include <string>
#include <d3dx9.h>

#define BLOOM_BLUR_SAMPLE_COUNT 15

class CTexture;
class CEffectTechnique;
class CEffect;

struct SBloomProcessSettings
{
  float     m_BloomThreshold;
  float     m_BlurAmount;
  float     m_BloomIntensity;
  float     m_BaseIntensity;
  float     m_BloomSaturation;
  float     m_BaseSaturation;

  //Constructor
  SBloomProcessSettings()
    : m_BloomThreshold( 0.25f )
    , m_BlurAmount( 4.0f )
    , m_BloomIntensity( 1.25f )
    , m_BaseIntensity( 1.0f )
    , m_BloomSaturation( 1.0f )
    , m_BaseSaturation( 1.0f )
  {
  }

  SBloomProcessSettings( float bloomThreshold, float blurAmount,
                         float bloomIntensity, float baseIntensity,
                         float bloomSaturation, float baseSaturation )
    : m_BloomThreshold( bloomThreshold )
    , m_BlurAmount( blurAmount )
    , m_BloomIntensity( bloomIntensity )
    , m_BaseIntensity( baseIntensity )
    , m_BloomSaturation( bloomSaturation )
    , m_BaseSaturation( baseSaturation )
  {
  }
};

class CBloomProcessSceneRenderCommand
{
private:
  CTexture*         m_Scene;
  CTexture*         m_RT1;
  CTexture*         m_RT2;
  CEffectTechnique*     m_BloomExtract;
  CEffectTechnique*     m_GaussianBlur;
  CEffectTechnique*     m_BloomCombine;
  CEffect*            m_BloomEffect;
  D3DXHANDLE          m_SceneTextureParameter;
  D3DXHANDLE          m_BloomThresholdParameter;
  D3DXHANDLE          m_SampleOffsetsParameter;
  D3DXHANDLE          m_SampleWeightsParameter;
  D3DXHANDLE          m_GaussianBlurTextureParameter;
  D3DXHANDLE          m_BloomIntensityParameter;
  D3DXHANDLE          m_BaseIntensityParameter;
  D3DXHANDLE          m_BloomSaturationParameter;
  D3DXHANDLE          m_BaseSaturationParameter;
  D3DXHANDLE          m_PostBloomTextureParameter;

  SBloomProcessSettings m_Settings;

  void            SetBlurParameters( float dx, float dy );
  float           ComputeGaussianBlur( float n );
  bool            RenderBloomExtract();
  bool            RenderBloomGaussianBlur();
  bool            RenderBloomCombine();

public:
  CBloomProcessSceneRenderCommand( const SBloomProcessSettings& settings );
  virtual ~CBloomProcessSceneRenderCommand();

  void            Init();
  void            Render();
};

#endif