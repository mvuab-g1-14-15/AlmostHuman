#include "RenderableObjectTechnique.h"

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique):
m_EffectTechnique(EffectTechnique)
{
    SetName(Name);
}

CRenderableObjectTechnique::~CRenderableObjectTechnique()
{
    CHECKED_DELETE(m_EffectTechnique);
}
