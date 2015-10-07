#pragma once
#ifndef INC_INSTANCE_MESH_H_
#define INC_INSTANCE_MESH_H_

#include "RenderableObject\RenderableObject.h"
#include "Utils\Defines.h"

class CStaticMesh;
class CGraphicsManager;

class CXMLTreeNode;
class CPhysicActor;
class CMaterial;

class CInstanceMesh : public CRenderableObject
{
    private:
        std::string   mType;
        CStaticMesh*  mStaticMesh;
        CPhysicActor* mPhysicActor;

        std::string   mMaterialName;
        CMaterial*    mMaterial;
        float         mAABBRadius;
        Math::Vect3f  mAABBCenter;
        bool          mIsDynamicMesh;

        std::vector<Math::Vect3f> Dummy01;
        std::vector<uint32> Dummy02;

    public:
        CInstanceMesh( const std::string& aName, const std::string& CoreName );
        CInstanceMesh( const std::string& aName );

        CInstanceMesh( const CXMLTreeNode& atts );
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

        void GetMaterial();
		void GetStaticMesh( const std::string& aCoreName );
        void GetCenterAndRadiusFromAABB();
};
#endif //INC_INSTANCE_MESH_H_
