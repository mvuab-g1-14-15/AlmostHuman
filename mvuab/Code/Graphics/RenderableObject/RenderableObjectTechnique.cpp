#include "RenderableObject\RenderableObjectTechnique.h"


CRenderableObjectTechnique::CRenderableObjectTechnique()
{
}

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string& Name, CEffectTechnique* EffectTechnique) : CName(Name), m_EffectTechnique(EffectTechnique)
{
}

CRenderableObjectTechnique::~CRenderableObjectTechnique()
{
}
