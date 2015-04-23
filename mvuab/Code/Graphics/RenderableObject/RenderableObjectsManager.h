#pragma once

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObject\RenderableObject.h"
#include "GraphicsManager.h"
#include <vector>

class CCinematic;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>
{
    private:

    public:
        CRenderableObjectsManager();
        ~CRenderableObjectsManager();

        void Update();
        void Render();

        /*CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Math::Vect3f &Position);
        CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Math::Vect3f &Position);*/

         void CleanUp();
         bool Load( const std::string& FileName );

         void RemoveResource(const std::string &l_Name);

        CCinematic* CreateCinematic(const std::string& FileName);
        CRenderableObject* GetInstance(const std::string& Name) const;
};