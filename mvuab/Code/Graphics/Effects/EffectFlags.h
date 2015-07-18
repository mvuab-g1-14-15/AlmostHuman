#pragma once
#ifndef EFFECT_FLAGS
#define EFFECT_FLAGS

#include "Utils\Types.h"
#include "XML\XMLTreeNode.h"

class CEffectFlags
{
public:
  CEffectFlags( const CXMLTreeNode& aFlags );
  virtual ~CEffectFlags();

  const uint32 GetMatrixFlags() const;

private:

  // Matrix Flags
  uint32 mMatrixFlags;

  // Lights
  uint32 mLightsFlags;
  uint32 mNumLights;
  
  // Fog
  bool                        m_UseFog;
  float                       m_FogStart;
  float                       m_FogEnd;
  float                       m_FogExp;
  EFogFunction                m_FogFun;

  // Textures
  bool                        m_UseTextureSizes;
  uint32                      m_TextureHeight;
  uint32                      m_TextureWidth;

  // Time
  uint32 mTimeFlags;

  // Debug
  bool m_UseDebugColor;

  void FillMatrixFlags(const CXMLTreeNode& aFlags );
  void FillLightsFlags(const CXMLTreeNode& aFlags );
  void FillFogFlags(const CXMLTreeNode& aFlags );
  void FillDebugFlags(const CXMLTreeNode& aFlags );
  void FillTimeFlags(const CXMLTreeNode& aFlags );
  void FillTextureFlags(const CXMLTreeNode& aFlags );
};
#endif // EFFECT_FLAGS