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

CInstanceMesh::CInstanceMesh( const std::string& aName, const std::string& CoreName )
	: CRenderableObject()
    , mType( "static" )
    , mPhysicActor( 0 )
    , mStaticMesh( SMeshMInstance->GetResource( CoreName ) )
{
  SetName( aName );
  mIsOk = mStaticMesh != NULL;
  mIsDynamic = mType == "dynamic";
  if( mIsOk )
  {
      mRadius = mStaticMesh->GetAABB().GetRadius();
      mCenter = mStaticMesh->GetAABB().GetCenter();

      if( !mIsDynamic )
      {
          mCenter = GetTransform() * mCenter;
      }
  }
}


CInstanceMesh::CInstanceMesh( const CXMLTreeNode& atts )
  : CRenderableObject( atts )
  , mStaticMesh( SMeshMInstance->GetResource( atts.GetAttribute<std::string>( "core", "no_staticMesh" ) ) )
  , mType( "static" )
  , mPhysicActor( 0 )
{
  mIsOk = mStaticMesh != NULL;
  mIsDynamic = mType == "dynamic";
  if( mIsOk )
  {
      mRadius = mStaticMesh->GetAABB().GetRadius();
      mCenter = mStaticMesh->GetAABB().GetCenter();

      if( !mIsDynamic )
      {
          mCenter = GetTransform() * mCenter;
      }
  }
}

CInstanceMesh::~CInstanceMesh()
{
	if( mIsDynamic  )
	{
		PhysXMInstance->ReleasePhysicActor( mPhysicActor );
	}
}

const std::vector<Math::Vect3f>& CInstanceMesh::GetVertexBuffer()
{
  return  mStaticMesh->GetVertexBuffer();
}

const std::vector<uint32>& CInstanceMesh::GetIndexBuffer()
{
  return mStaticMesh->GetIndexBuffer();
}

void CInstanceMesh::Render()
{
  if ( !mStaticMesh || !GetActive() )
    return;

  Math::Mat44f lTransform = GetTransform();
  mCenter = lTransform* mStaticMesh->GetAABB().GetCenter();

  if( mIsDynamic )
  {    
      mPhysicActor->GetMat44( lTransform );
      Math::Vect3f lUp( 0.0f, -mStaticMesh->GetAABB().GetCenter().y, 0.0f );

      Math::Mat44f lCenterTransform;
      lCenterTransform.SetIdentity();

      lCenterTransform.Translate( lUp );
      lTransform = lTransform * lCenterTransform;
  }

  CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
  CGraphicsManager* lGM = GraphicsInstance;
  if ( lCameraFrustum.SphereVisible( D3DXVECTOR3( mCenter.u ), mRadius ) )
  {
    lGM ->SetTransform( lTransform );
    mStaticMesh->Render( lGM  );
    lGM ->SetTransform( Math::Mat44f() );
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