#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_H
#define RENDERABLE_OBJECT_TECHINQUE_H

#include "Utils\Name.h"

class CEffectTechnique;

class CRenderableObjectTechnique : public CName
{
private:
    CEffectTechnique *m_EffectTechnique;
public:
	CRenderableObjectTechnique();
    CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);
    ~CRenderableObjectTechnique();
    void SetEffectTechnique(CEffectTechnique *EffectTechnique){ m_EffectTechnique = EffectTechnique;}
	CEffectTechnique * GetEffectTechnique() const { return m_EffectTechnique;}
};

#endif