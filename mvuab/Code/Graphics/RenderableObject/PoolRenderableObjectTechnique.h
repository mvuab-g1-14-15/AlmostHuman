#pragma once
#ifndef POOL_RENDERABLE_OBJECT_TECHINQUE_H
#define POOL_RENDERABLE_OBJECT_TECHINQUE_H

#include "Effects\EffectTechnique.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "Utils\Name.h"
#include "XML\XMLTreeNode.h"

#include <vector>


class CPoolRenderableObjectTechnique : public CName
{
public:
    CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);
    virtual ~CPoolRenderableObjectTechnique();
    
    void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
    void Destroy();
    void Apply();

private:
    class CPoolRenderableObjectTechniqueElement
    {
        public:
            CRenderableObjectTechnique m_RenderableObjectTechnique;
            CRenderableObjectTechnique *m_OnRenderableObjectTechniqueManager;
            CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager);
    };
    std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;
};

#endif