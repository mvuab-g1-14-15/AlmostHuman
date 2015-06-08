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
private:
    std::string   mType;
    CStaticMesh*  mStaticMesh;
    CPhysicActor* mPhysicActor;

    std::vector<Math::Vect3f> m_VB;
    std::vector<uint32> m_IB;

public:
    CInstanceMesh( const std::string& aName, const std::string& CoreName );
    CInstanceMesh( const std::string& aName );

    CInstanceMesh( CXMLTreeNode& atts );
    ~CInstanceMesh();

    void Render();
    void SetActor( CPhysicActor* lPhysicActor );
	CPhysicActor* GetActor();

    const std::vector<Math::Vect3f>& GetVertexBuffer();
    const std::vector<uint32>& GetIndexBuffer();

    const std::string& GetType()
    {
        return mType;
    }
    void SetType( const std::string& lType )
    {
        mType = lType;
    }
    CStaticMesh* GetStaticMesh();
};
#endif //INC_INSTANCE_MESH_H_
