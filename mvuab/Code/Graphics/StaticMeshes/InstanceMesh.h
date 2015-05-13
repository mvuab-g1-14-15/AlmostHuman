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
        CInstanceMesh(const std::string& Name, const std::string &CoreName);
        CInstanceMesh( const std::string& aName );
        CInstanceMesh(CXMLTreeNode& atts);
        ~CInstanceMesh();

        std::vector<Math::Vect3f> GetVertices() { return m_VB; }
        std::vector<uint32> GetIndices() { return m_IB; }
        
        bool Init( CPhysicActor* aActor, CStaticMesh* aStaticMesh );

        void Render();
        inline CStaticMesh* GetStaticMesh();
    private:
        CStaticMesh*        mStaticMesh;
        CPhysicActor*       mPhysicActor;

        std::vector<Math::Vect3f> m_VB;
        std::vector<uint32> m_IB;
};

//--------------------------------------------------------------------------------
inline CStaticMesh* CInstanceMesh::GetStaticMesh()
{
    return mStaticMesh;
}

#endif //INC_INSTANCE_MESH_H_
