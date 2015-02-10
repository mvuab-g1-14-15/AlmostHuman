#include "RenderableObject\RenderableObjectTechnique.h"


CRenderableObjectTechnique::CRenderableObjectTechnique()
{

}

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique):
m_EffectTechnique(EffectTechnique)
{
    SetName(Name);
}

CRenderableObjectTechnique::~CRenderableObjectTechnique()
{
 
}
