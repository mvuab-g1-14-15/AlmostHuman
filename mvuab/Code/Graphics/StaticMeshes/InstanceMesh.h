#pragma once
#ifndef INC_INSTANCE_MESH_H_
#define INC_INSTANCE_MESH_H_

#include "RenderableObject\RenderableObject.h"
#include "Utils\Defines.h"

class CStaticMesh;
class CGraphicsManager;
class CXMLTreeNode;
class CPhysicActor;

class CInstanceMesh : public CRenderableObject
{
    public:
        CInstanceMesh(const std::string& aName, const std::string &CoreName);
        CInstanceMesh(const std::string& aName);

        CInstanceMesh(CXMLTreeNode& atts);
        ~CInstanceMesh();

        void Render();
        inline CStaticMesh* GetStaticMesh();

    private:
        CStaticMesh*        mStaticMesh;

        std::vector<Math::Vect3f> m_VB;
        std::vector<uint32> m_IB;
};


#endif //INC_INSTANCE_MESH_H_
