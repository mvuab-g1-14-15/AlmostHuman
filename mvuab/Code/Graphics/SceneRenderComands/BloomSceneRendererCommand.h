#ifndef BLOOM_SCENE_RENDERER_COMMAND_
#define BLOOM_SCENE_RENDERER_COMMAND_
#pragma once

#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Effects\Defines.h"
#include "Texture\Texture.h"

class CGraphicsManager;
class CEffectTechnique;

class CBloomSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
    CBloomSceneRendererCommand( CXMLTreeNode& atts );
    ~CBloomSceneRendererCommand();
    void Execute( CGraphicsManager& GM );

private:
  CEffectTechnique *m_Technique;
  LPD3DXEFFECT          m_Effect;
  float     mBlurAmount;
  float     mBloomThreshold;
  float     mBloomIntensity;
  float     mBloomOriginalIntensity;
  float     mBloomSaturation;
  float     mBloomOriginalSaturation;
  std::string mStage;
  DECLARE_EFFECT_PARAMETER(BloomThreshold);
  DECLARE_EFFECT_PARAMETER(BloomIntensity);
  DECLARE_EFFECT_PARAMETER(BloomOriginalIntensity);
  DECLARE_EFFECT_PARAMETER(BloomSaturation);
  DECLARE_EFFECT_PARAMETER(BloomOriginalSaturation);
  

  void LinkBloomParameters();
  void GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle );
  void GetParameterBySemantic( const std::string& SemanticName, D3DXHANDLE& a_Handle );
  bool ParseXML(CXMLTreeNode& att);
};
#endif