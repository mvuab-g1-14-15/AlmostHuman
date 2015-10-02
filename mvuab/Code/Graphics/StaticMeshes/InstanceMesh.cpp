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

#include "Material.h"

#include <algorithm>
#include <string> 

CInstanceMesh::CInstanceMesh( const std::string& aName ) : CRenderableObject(), mStaticMesh( 0 ) , mType( "static" ), mPhysicActor( 0 )
{
  SetName( aName );
  mMaterialName = aName;
  GetMaterial();
  GetCenterAndRadiusFromAABB();
}


CInstanceMesh::CInstanceMesh( const std::string& aName, const std::string& CoreName, const std::string& MaterialName ) : mStaticMesh( SMeshMInstance->GetResource( CoreName ) ),
  CRenderableObject(), mType( "static" ), mPhysicActor( 0 )
{
  SetName(aName);
  if (mMaterialName == "")
    mMaterialName = aName;
  else
    mMaterialName = MaterialName;
  GetMaterial();
  GetCenterAndRadiusFromAABB();
}


CInstanceMesh::CInstanceMesh( const CXMLTreeNode& atts )
  : CRenderableObject( atts )
  , mStaticMesh( SMeshMInstance->GetResource( atts.GetAttribute<std::string>( "core", "no_staticMesh" ) ) )
  , mType( "static" )
  , mPhysicActor( 0 )
{
  mMaterialName = GetName();
  GetMaterial();
  GetCenterAndRadiusFromAABB();
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
  return mStaticMesh == 0 ? Dummy01 : mStaticMesh->GetVertexBuffer();
}

const std::vector<uint32>& CInstanceMesh::GetIndexBuffer()
{
  return  mStaticMesh == 0 ? Dummy02 :  mStaticMesh->GetIndexBuffer();
}

void CInstanceMesh::Render()
{
  if ( mStaticMesh && GetActive() )
  {
    Math::Mat44f lTransform = GetTransform();
    if( mIsDynamicMesh && mPhysicActor )
    {
      mPhysicActor->GetMat44( lTransform );
      Math::Vect3f lUp( 0.0f, -mStaticMesh->GetAABBCenter().y, 0.0f );

      Math::Mat44f lCenterTransform;
      lCenterTransform.SetIdentity();

      lCenterTransform.Translate( lUp );
      lTransform = lTransform * lCenterTransform;

      mAABBCenter = lTransform * mAABBCenter;
    }

    // Check the visibility with the frustum
    if ( CameraMInstance->GetCurrentCamera()->GetFrustum().SphereVisible( D3DXVECTOR3( mAABBCenter.u ), mAABBRadius ) )
    {
      CGraphicsManager* lGM = GraphicsInstance;
      lGM->SetTransform( lTransform );
      for( uint32 i = 0, lCount = mMaterial->GetCount(); i < lCount; ++i )
      {
        mMaterial->ApplyMaterial(i);
        mStaticMesh->Render( lGM, i );
      }
      lGM->SetTransform(Math::Mat44f());
    }

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

void CInstanceMesh::GetMaterial()
{
  std::string& lMaterialName = mMaterialName;
  std::string::size_type i = lMaterialName.find_last_of("_");
  if (i != std::string::npos)
    lMaterialName.erase(i, lMaterialName.size() - i);

  std::transform(lMaterialName.begin(), lMaterialName.end(), lMaterialName.begin(), ::tolower);

  mMaterial = SMeshMInstance->GetMaterial( lMaterialName );

  ASSERT(mMaterial, "Null material");
}

//---------------------------------------------------------------------------------------------------------------------
void CInstanceMesh::GetCenterAndRadiusFromAABB()
{
  if( mStaticMesh )
  {
    mIsDynamicMesh = ( mType == "dynamic");
    mAABBCenter = GetTransform() * mStaticMesh->GetAABBCenter();
    mAABBRadius = mStaticMesh->GetAABB().GetRadius() * 2.0f;
  }
}