#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_H
#define RENDERABLE_OBJECT_TECHINQUE_H

#include "Effects\EffectTechnique.h"
#include "Utils\Name.h"

class CRenderableObjectTechnique : public CName
{
private:
    CEffectTechnique *m_EffectTechnique;
public:
    CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);
    ~CRenderableObjectTechnique();
    void SetEffectTechnique(CEffectTechnique *EffectTechnique){ m_EffectTechnique = EffectTechnique;}
    CEffectTechnique * GetEffectTechnique() const { return m_EffectTechnique;}
};

#endif