#pragma once
#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "d3dx9.h"
#include "XML\XMLTreeNode.h"
#include <string>

class CEffect;

class CEffectTechnique
{
public:
    CEffectTechnique(CXMLTreeNode & node);
    virtual ~CEffectTechnique();
    
    inline CEffect* GetEffect() const {return m_Effect;}
    bool BeginRender();
    bool Refresh();
    //DirectX Methods Interface
    D3DXHANDLE GetD3DTechnique() {return m_D3DTechnique;}

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
    CEffect                    *m_Effect;
    D3DXHANDLE                  m_D3DTechnique;
    std::string                 m_TechniqueName;
    std::string                 m_EffectName;
private: // Methods
    void SetupMatrices();
    bool SetupLights();
};
#endif // EFFECT_TECHNIQUE_H