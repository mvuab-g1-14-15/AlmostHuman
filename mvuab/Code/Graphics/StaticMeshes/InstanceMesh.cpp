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

CInstanceMesh::CInstanceMesh( const std::string& aName ) : CRenderableObject(), mStaticMesh( 0 ) , mType( "static" ), mPhysicActor( 0 )
{
  SetName( aName );
}

CInstanceMesh::CInstanceMesh( const std::string& aName, const std::string& CoreName ) : mStaticMesh( SMeshMInstance->GetResource( CoreName ) ),
  CRenderableObject(), mType( "static" ), mPhysicActor( 0 )
{
  SetName( aName );
}

CInstanceMesh::CInstanceMesh( CXMLTreeNode& atts ) : CRenderableObject( atts ), mStaticMesh( SMeshMInstance->GetResource( atts.GetPszProperty( "core",
      "no_staticMesh" ) ) ), mType( "static" ), mPhysicActor( 0 )
{
}

CInstanceMesh::~CInstanceMesh()
{
}

const std::vector<Math::Vect3f>& CInstanceMesh::GetVertexBuffer()
{
  return mStaticMesh->GetVertexBuffer();
}

const std::vector<uint32>& CInstanceMesh::GetIndexBuffer()
{
  return mStaticMesh->GetIndexBuffer();
}

void CInstanceMesh::Render()
{
  if ( !mStaticMesh )
    return;

  Math::Mat44f lTransform = GetTransform();
  Math::AABB3f laabb = mStaticMesh->GetAABB();

  if ( mType == "dynamic" && mPhysicActor != 0 )
  {
    mPhysicActor->GetMat44( lTransform );

	float lMaxDistance = (laabb.GetMax().y - laabb.GetCenter().y);
	Math::Vect3f lUp( 0.0f, - laabb.GetCenter().y, 0.0f );
	Math::Vect3f lCenter = laabb.GetCenter();
	Math::Mat44f lCenterTransform;
	lCenterTransform.SetIdentity();
	
	lCenterTransform.Translate( lUp );

	lTransform = lTransform * lCenterTransform;
	
  }

  GraphicsInstance->SetTransform( lTransform );

  CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
  Math::Vect3f lPositionTransformed = lTransform * laabb.GetCenter();

  if ( lCameraFrustum.SphereVisible( D3DXVECTOR3( lPositionTransformed.x, lPositionTransformed.y, lPositionTransformed.z ), laabb.GetRadius() ) )
    mStaticMesh->Render( GraphicsInstance );
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