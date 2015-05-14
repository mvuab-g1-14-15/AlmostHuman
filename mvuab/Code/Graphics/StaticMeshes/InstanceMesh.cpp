#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "InstanceMesh.h"

#include "Math\AABB.h"
#include "GraphicsManager.h"

#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

#include "NxPhysicsSDK.h"
#include "PhysicsManager.h"
#include "EngineManagers.h"

#include "NxPhysics.h"
#include "NxCooking.h"

#include "NxTriangleMesh.h"
#include "NxTriangleMeshDesc.h"

#include "Actor\PhysicActor.h"
#include "Utils\PhysicStream.h"
#include "Utils\PhysicUserData.h"


CInstanceMesh::CInstanceMesh( const std::string& aName ) : CRenderableObject(), mStaticMesh( 0 ) 
{
    SetName(aName);
}

CInstanceMesh::CInstanceMesh(const std::string& aName, const std::string& CoreName) : mStaticMesh(SMeshMInstance->GetResource(CoreName)) , CRenderableObject()
{
    ASSERT( mStaticMesh, "Null static mesh" );
    
    if(mStaticMesh != 0)
    {
        m_VB = mStaticMesh->GetVB();
        m_IB = mStaticMesh->GetIB();

        Math::Mat44f l_Transform = GetTransform();
        int l_VecElements = m_VB.size();

        #pragma omp parallel for
        for(int i = 0; i < l_VecElements; ++i)
        {
            m_VB[i] = l_Transform * m_VB[i];
        }

        NxTriangleMeshDesc l_TriangleMeshDesc;

        l_TriangleMeshDesc.numVertices      = ( NxU32 ) m_VB.size();
        l_TriangleMeshDesc.numTriangles     = ( NxU32 ) m_IB.size() / 3;
        
        l_TriangleMeshDesc.pointStrideBytes     = sizeof( Math::Vect3f );
        l_TriangleMeshDesc.triangleStrideBytes  = 3 * sizeof( uint32 );
        
        l_TriangleMeshDesc.points         = &m_VB[0].x;
        l_TriangleMeshDesc.triangles      = &m_IB[0];
        l_TriangleMeshDesc.flags          = 0;
        
        CPhysicMemoryWriteBuffer buf;
        NxCookingInterface *l_pCooking = NxGetCookingLib( NX_PHYSICS_SDK_VERSION );
        
        if(l_pCooking->NxCookTriangleMesh(l_TriangleMeshDesc, buf))
        {
            NxTriangleMesh* l_TriangleMesh = PhysXMInstance->GetPhysicsSDK()->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
            
            if(l_TriangleMesh != 0)
            {
                CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData(aName);
                CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataASEMesh);

                l_MeshActor->AddMeshShape(l_TriangleMesh, Math::Vect3f());
                bool oK = false;
                
                if(PhysXMInstance->CMapManager<CPhysicActor>::GetResource(aName) == 0)
                {
                    if(PhysXMInstance->AddPhysicActor(l_MeshActor))
                    {
                        PhysXMInstance->CMapManager<CPhysicActor>::AddResource(aName, l_MeshActor);
                        oK = TRUE;
                    }
                }

                if(!oK)
                {
                    CHECKED_DELETE(l_MeshActor);
                    CHECKED_DELETE(l_pPhysicUserDataASEMesh);
                }
            }
        }
    }

    SetName( aName );
}

CInstanceMesh::CInstanceMesh( CXMLTreeNode& atts ) : CRenderableObject( atts ), mStaticMesh( SMeshMInstance->GetResource(atts.GetPszProperty( "core", "no_staticMesh" ) ) )
{
    if(mStaticMesh != 0)
    {
        std::vector<Math::Vect3f> m_VB = mStaticMesh->GetVB();
        std::vector<uint32> m_IB = mStaticMesh->GetIB();

        Math::Mat44f l_Transform = GetTransform();
        int l_VecElements = m_VB.size();

        #pragma omp parallel for
        for(int i = 0; i < l_VecElements; ++i)
        {
            m_VB[i] = l_Transform * m_VB[i];
        }

        NxTriangleMeshDesc l_TriangleMeshDesc;

        l_TriangleMeshDesc.numVertices      = ( NxU32 ) m_VB.size();
        l_TriangleMeshDesc.numTriangles     = ( NxU32 ) m_IB.size() / 3;
        
        l_TriangleMeshDesc.pointStrideBytes     = sizeof( Math::Vect3f );
        l_TriangleMeshDesc.triangleStrideBytes  = 3 * sizeof( uint32 );
        
        l_TriangleMeshDesc.points         = &m_VB[0].x;
        l_TriangleMeshDesc.triangles      = &m_IB[0];
        l_TriangleMeshDesc.flags          = 0;
        
        CPhysicMemoryWriteBuffer buf;
        NxCookingInterface *l_pCooking = NxGetCookingLib( NX_PHYSICS_SDK_VERSION );
        
        if(l_pCooking->NxCookTriangleMesh(l_TriangleMeshDesc, buf))
        {
            NxTriangleMesh* l_TriangleMesh = PhysXMInstance->GetPhysicsSDK()->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
            
            if(l_TriangleMesh != 0)
            {
                CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData(GetName());
                CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataASEMesh);

                l_MeshActor->AddMeshShape(l_TriangleMesh, Math::Vect3f());
                bool oK = false;
                
                if(PhysXMInstance->CMapManager<CPhysicActor>::GetResource(GetName()) == 0)
                {
                    if(PhysXMInstance->AddPhysicActor(l_MeshActor))
                    {
                        PhysXMInstance->CMapManager<CPhysicActor>::AddResource(GetName(), l_MeshActor);
                        oK = TRUE;
                    }
                }

                if(!oK)
                {
                    CHECKED_DELETE(l_MeshActor);
                    CHECKED_DELETE(l_pPhysicUserDataASEMesh);
                }
            }
        }
    }
}

CInstanceMesh::~CInstanceMesh()
{
}

void CInstanceMesh::Render()
{
    if (!mStaticMesh) { return; }

    Math::Mat44f lTransform = GetTransform();
    GraphicsInstance->SetTransform( lTransform );
    Math::AABB3f laabb = mStaticMesh->GetAABB();

    CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
    Math::Vect3f lPositionTransformed = lTransform * laabb.GetCenter();

    // TODO: Fix the frustum culling
    if(lCameraFrustum.SphereVisible( D3DXVECTOR3(lPositionTransformed.x, lPositionTransformed.y, lPositionTransformed.z), laabb.GetRadius()) )
    { mStaticMesh->Render( GraphicsInstance ); }

    Math::Mat44f t;
    GraphicsInstance->SetTransform( t );
}
