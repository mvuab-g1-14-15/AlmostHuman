#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "InstanceMesh.h"

#include "Math\AABB.h"
#include "Actor\PhysicActor.h"

#include "EngineManagers.h"
#include "GraphicsManager.h"

#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

CInstanceMesh::CInstanceMesh(const std::string& aName) : CRenderableObject(), mStaticMesh(0), mType("static")
{
    SetName(aName);
}

CInstanceMesh::CInstanceMesh(const std::string& aName, const std::string& CoreName) : mStaticMesh(SMeshMInstance->GetResource(CoreName)), CRenderableObject(), mType("static")
{
    SetName( aName );
}

CInstanceMesh::CInstanceMesh(CXMLTreeNode& atts) : CRenderableObject(atts), mStaticMesh( SMeshMInstance->GetResource(atts.GetPszProperty("core", "no_staticMesh"))), mType("static")
{
}

CInstanceMesh::~CInstanceMesh()
{
}

const std::vector<Math::Vect3f> &CInstanceMesh::GetVertexBuffer() 
{ 
    return mStaticMesh->GetVertexBuffer(); 
}

const std::vector<uint32> &CInstanceMesh::GetIndexBuffer() 
{ 
    return mStaticMesh->GetIndexBuffer(); 
}

void CInstanceMesh::Render()
{
    if (!mStaticMesh) { return; }

    Math::Mat44f lTransform = GetTransform();

    if(mType == "dynamic")
    {
        //mPhysicActor->GetMat44(lTransform);
    }

    GraphicsInstance->SetTransform( lTransform );
    Math::AABB3f laabb = mStaticMesh->GetAABB();

    CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
    Math::Vect3f lPositionTransformed = lTransform * laabb.GetCenter();

    if(lCameraFrustum.SphereVisible( D3DXVECTOR3(lPositionTransformed.x, lPositionTransformed.y, lPositionTransformed.z), laabb.GetRadius()) )
    { mStaticMesh->Render( GraphicsInstance ); }

    Math::Mat44f t;
    GraphicsInstance->SetTransform( t );
}

void CInstanceMesh::SetActor(CPhysicActor *lPhysicActor)
{
    mPhysicActor = lPhysicActor;
}
