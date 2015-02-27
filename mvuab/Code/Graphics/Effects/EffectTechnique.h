#pragma once
#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "d3dx9.h"
#include "XML\XMLTreeNode.h"
#include "Math/Color.h"
#include <string>
#include "Utils\Defines.h"

class CEffect;

class CEffectTechnique
{
public:
  CEffectTechnique( const std::string& TechniqueName, const std::string& EffectName, CXMLTreeNode& HandlesNode );
  virtual ~CEffectTechnique();

  inline CEffect* GetEffect() const
  {
    return m_Effect;
  }
  bool BeginRender();
  bool Refresh();
  //DirectX Methods Interface
  D3DXHANDLE GetD3DTechnique()
  {
    return m_D3DTechnique;
  }

  void SetDebugColor( Math::CColor color = Math::colWHITE );

  void SetUseResolution(bool active = false);
  void SetTextureSize(unsigned int width, unsigned int height);

private: // Members
  bool                        m_UseCameraPosition;
  bool                        m_UseInverseProjMatrix;
  bool                        m_UseInverseViewMatrix;
  bool                        m_UseInverseWorldMatrix;
  bool                        m_UseLights;
  int32                       m_NumOfLights;
  bool                        m_UseLightAmbientColor;
  bool                        m_UseProjMatrix;
  bool                        m_UseViewMatrix;
  bool                        m_UseWorldMatrix;
  bool                        m_UseWorldViewMatrix;
  bool                        m_UseWorldViewProjectionMatrix;
  bool                        m_UseViewProjectionMatrix;
  bool                        m_UseViewToLightProjectionMatrix;
  bool                        m_UseTime;
  bool                        m_UseDebugColor;
  bool						  m_UseTextureSizesGaussian;

  uint32					  m_TextureHeight;
  uint32					  m_TextureWidth;

  float                       m_FogStart;
  float                       m_FogEnd;
  float                       m_FogExp;
  int                         m_FogFun;
  CEffect*                    m_Effect;
  D3DXHANDLE                  m_D3DTechnique;
  std::string                 m_TechniqueName;
  std::string                 m_EffectName;

  Math::CColor        m_DebugColor;
private: // Methods
  void SetupMatrices();
  bool SetupLights();
};
#endif // EFFECT_TECHNIQUE_H