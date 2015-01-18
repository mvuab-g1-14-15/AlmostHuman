#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_H
#define RENDERABLE_OBJECT_TECHINQUE_H

#include "Effects\EffectTechnique.h"
#include "Utils\Name.h"

class CRenderableObjectTechnique : public CName
{
private:
    CEffectTechnique *m_EffectTechnique;
    uint32 m_VertexType;
public:
    CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique, uint32 VertexType);
    ~CRenderableObjectTechnique();
    void SetEffectTechnique(CEffectTechnique *EffectTechnique){ m_EffectTechnique = EffectTechnique;}
    CEffectTechnique * GetEffectTechnique() const { return m_EffectTechnique;}
    GET_SET(uint32, VertexType);
};

#endif