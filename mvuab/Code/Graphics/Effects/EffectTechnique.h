#pragma once
#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "XML\XMLTreeNode.h"
#include "Math/Color.h"
#include <string>
#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Effects\Defines.h"
#include "dx9.h"

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

  void SetUseTextureSize( bool active = false );
  void SetTextureSize( unsigned int width, unsigned int height );

private: // Members

  // Debug
  Math::CColor                m_DebugColor;

  // Lights
  int32                       m_NumOfLights;

  // Textures
  uint32                      m_TextureHeight;
  uint32                      m_TextureWidth;

  // Fog
  float                       m_FogStart;
  float                       m_FogEnd;
  float                       m_FogExp;
  EFogFunction                m_FogFun;

  CEffect*                    m_Effect;
  D3DXHANDLE                  m_D3DTechnique;
  std::string                 m_TechniqueName;
  std::string                 m_EffectName;

  bool mUseWorld;
  bool mUseInverseWorld;
  bool mUseView;
  bool mUseInverseView;
  bool mUseProjection;
  bool mUseInverseProjection;
  bool mUseWorldView;
  bool mUseViewProjection;
  bool mUseWorldViewProjection;
  bool mUseViewToLightProjection;
  bool mUseFog;
  bool mUseLights;
  bool mUseDebugColor;
  bool mUseFBSize;
  bool mUseAmbientLight;
  bool mUseCamera;
  bool mUseTime;

private: // Methods
  void SetupMatrices();
  bool SetupLights();
  void ReadFlags( const CXMLTreeNode& aFlagsNode );
};
#endif // EFFECT_TECHNIQUE_H