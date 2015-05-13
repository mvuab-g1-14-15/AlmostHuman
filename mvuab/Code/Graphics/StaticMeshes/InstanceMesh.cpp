#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "InstanceMesh.h"

#include "Math\AABB.h"
#include "GraphicsManager.h"

#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

#include "EngineManagers.h"

#include "Actor/PhysicActor.h"


CInstanceMesh::CInstanceMesh( const std::string& aName )
    : CRenderableObject()
    , mStaticMesh( 0 ) 
    , mPhysicActor( 0 )
{
    SetName( aName );
}

CInstanceMesh::CInstanceMesh( const std::string& Name, const std::string& CoreName )
    : mStaticMesh( SMeshMInstance->GetResource(CoreName ) )
    , CRenderableObject()
    , mPhysicActor( 0 )
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
    }

    SetName( Name );
}

CInstanceMesh::CInstanceMesh( CXMLTreeNode& atts ) : CRenderableObject( atts ), mStaticMesh( SMeshMInstance->GetResource(atts.GetPszProperty( "core", "no_staticMesh" ) ) )
{
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

bool CInstanceMesh::Init( CPhysicActor* aActor, CStaticMesh* aStaticMesh )
{
    mPhysicActor = aActor;
    mStaticMesh = aStaticMesh;
    return mStaticMesh != 0;
}