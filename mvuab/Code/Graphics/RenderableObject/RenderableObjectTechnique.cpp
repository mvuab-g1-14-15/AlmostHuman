#include "RenderableObject\RenderableObjectTechnique.h"

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique, uint32 VertexType):
m_EffectTechnique(EffectTechnique), m_VertexType(VertexType)
{
    SetName(Name);
}

CRenderableObjectTechnique::~CRenderableObjectTechnique()
{
    CHECKED_DELETE(m_EffectTechnique);
}
