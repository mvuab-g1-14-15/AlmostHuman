#pragma once

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObject\RenderableObject.h"
#include "GraphicsManager.h"
#include <vector>

class CCinematic;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>
{
    private:
        unsigned int m_NumVertexs;
        unsigned int m_NumFaces;
        unsigned int m_NumDraws;

    public:
        CRenderableObjectsManager();
        ~CRenderableObjectsManager();

        void Update();
        void Render();

        CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Math::Vect3f &Position);
        CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Math::Vect3f &Position);
		CCinematic* CreateCinematic(const std::string &FileName);

        void CleanUp();
        bool Load(const std::string &FileName);

        CRenderableObject * GetInstance(const std::string &Name) const;

        unsigned int        GetNumVertex        () { return m_NumVertexs; }
        unsigned int        GetNumFaces         () { return m_NumFaces; }
        unsigned int        GetNumDraws         () { return m_NumDraws; }
};