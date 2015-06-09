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

    Math::Vect3f l_Direction( Math::Utils::Cos( lTransform.GetAngleX() ) * Math::Utils::Cos( lTransform.GetAngleY() ),
                              Math::Utils::Sin( lTransform.GetAngleY() ),
                              Math::Utils::Sin( lTransform.GetAngleX() ) * Math::Utils::Cos( lTransform.GetAngleY() ) );
    Math::Vect3f lPos = lTransform.GetPos();

    if ( l_Direction.y == 1.0f && l_Direction.x < 0.0f && l_Direction.z < 0.0f )
      lPos.y = lPos.y - laabb.GetWidth() * 0.5f;

    if ( l_Direction.x >= 0.9f )
      lPos.y = lPos.y - laabb.GetHeight() * 0.5f;

    lTransform.SetPos( lPos );


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