#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "InstanceMesh.h"

#include "Math\AABB.h"
#include "Actor\PhysicActor.h"

#include "EngineManagers.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"

#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

CInstanceMesh::CInstanceMesh( const std::string& aName )
    : CRenderableObject( aName )
    , mStaticMesh( 0 )
    , mType( "static" )
    , mPhysicActor( 0 )
{
}


CInstanceMesh::CInstanceMesh( const std::string& aName, const std::string& CoreName )
    : CRenderableObject( aName )
    , mType( "static" )
    , mPhysicActor( 0 )
    , mStaticMesh( SMeshMInstance->GetResource( CoreName ) )
{
    mIsOk = mStaticMesh != NULL;
}


CInstanceMesh::CInstanceMesh( const CXMLTreeNode& atts ) 
    : CRenderableObject( atts )
    , mStaticMesh( SMeshMInstance->GetResource( atts.GetAttribute<std::string>( "core", "no_staticMesh" ) ) )
    , mType( atts.GetAttribute<std::string>( "type", "static" ) )
    , mPhysicActor( 0 )
{
    mIsOk = mStaticMesh != NULL;
}

CInstanceMesh::~CInstanceMesh()
{
	if( mType == "dynamic" )
	{
		PhysXMInstance->ReleasePhysicActor( mPhysicActor );
	}
}

const std::vector<Math::Vect3f>& CInstanceMesh::GetVertexBuffer()
{
    ASSERT(mStaticMesh, "Null Static Mesh for instance mesh %s", GetName().c_str() );
    return mStaticMesh->GetVertexBuffer();
}

const std::vector<uint32>& CInstanceMesh::GetIndexBuffer()
{
    ASSERT(mStaticMesh, "Null Static Mesh for instance mesh %s", GetName().c_str() );
    return mStaticMesh->GetIndexBuffer();
}

void CInstanceMesh::Render()
{
  if ( !mStaticMesh )
    return;

  if ( !GetActive() )
    return;

  bool l_Transfomed = mStaticMesh->IsTransformed();
  bool l_ObjDynamic = mType.compare( "dynamic" ) == 0;

  Math::Mat44f lTransform = GetTransform();
  Math::AABB3f laabb = mStaticMesh->GetAABB();

  Math::Vect3f laabbCenter =  mStaticMesh->GetAABBCenter();
  if ( !l_Transfomed ) laabbCenter = laabb.GetCenter();

  if ( l_ObjDynamic && ( mPhysicActor != 0 ) )
  {
    mPhysicActor->GetMat44( lTransform );
    Math::Vect3f lUp( 0.0f, -laabbCenter.y, 0.0f );

    Math::Mat44f lCenterTransform;
    lCenterTransform.SetIdentity();

    lCenterTransform.Translate( lUp );
    lTransform = lTransform * lCenterTransform;
  }

  if ( !l_Transfomed || l_ObjDynamic )
  {
    laabbCenter = lTransform * laabbCenter;
    mStaticMesh->setIsTransformed( true );
    mStaticMesh->SetAABB( laabbCenter );
  }

  CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();

  if ( lCameraFrustum.SphereVisible( D3DXVECTOR3( laabbCenter.u ), laabb.GetRadius() ) )
  {
    GraphicsInstance->SetTransform( lTransform );
    mStaticMesh->Render( GraphicsInstance );
  }
}

void CInstanceMesh::SetActor( CPhysicActor* lPhysicActor )
{
  mPhysicActor = lPhysicActor;
}

CPhysicActor* CInstanceMesh::GetActor()
{
  return mPhysicActor;
}


CStaticMesh* CInstanceMesh::GetStaticMesh()
{
  return mStaticMesh;
}