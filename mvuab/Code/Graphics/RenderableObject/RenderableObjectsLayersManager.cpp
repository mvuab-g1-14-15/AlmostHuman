#include "AnimatedModels\AnimatedInstanceModel.h"
#include "AnimatedModels\AnimatedInstanceModel.h"

#include "RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"

#include "EngineConfig.h"
#include "Utils\Defines.h"

#include "PhysicsManager.h"
#include "EngineManagers.h"

#include "StaticMeshes\StaticMeshManager.h"
#include "CookingMesh\PhysicCookingMesh.h"

#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Math\AABB.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"


CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
    : m_DefaultRenderableObjectManager( 0 )
{
}

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager( const CXMLTreeNode& atts )
    : m_DefaultRenderableObjectManager( 0 )
{
}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
    Destroy();

    for ( unsigned int i = 0; i < m_PhyscsUserData.size(); i++ )
        CHECKED_DELETE( m_PhyscsUserData.at( i ) );
    m_PhyscsUserData.clear();
}
void CRenderableObjectsLayersManager::Destroy()
{
    CTemplatedVectorMapManager::Destroy();
}

bool CRenderableObjectsLayersManager::LoadLayers( const std::string& l_FilePath, const std::string& l_RoomName )
{
	bool lOk = true;

    CXMLTreeNode l_Node, l_Root;
    if ( l_Root.LoadAndFindNode( l_FilePath.c_str(), "room", l_Node ) )
	{
		const std::string& l_Path = l_Root.GetAttribute<std::string>( "path", "no_path" );
		for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
		{
			const CXMLTreeNode& l_CurrentNode = l_Node( i );
			const std::string& l_Name = l_CurrentNode.GetAttribute<std::string>( "name", "no_path" );
			const std::string& l_File = l_CurrentNode.GetAttribute<std::string>( "file", "no_file" );
			lOk = lOk && LoadRenderableObject( l_Path + "/" + l_File, l_Name, l_RoomName );
		}
	}

    return lOk;
}

bool CRenderableObjectsLayersManager::LoadRenderableObject( const std::string& l_FilePath, const std::string& l_Name, const std::string& l_RoomName )
{
	bool lOk = true;
    CXMLTreeNode l_Node, l_Root;
    if ( l_Root.LoadAndFindNode( l_FilePath.c_str(), "RenderableObjects", l_Node ) )
	{
		uint32 l_NumChilds = l_Node.GetNumChildren();
		CRenderableObjectsManager* lRenderableObjectManager = new CRenderableObjectsManager();
		m_ResourcesVector.reserve( l_NumChilds );
		for ( uint32 i = 0; i < l_NumChilds; ++i )
		{
			const CXMLTreeNode& lNode = l_Node( i );
			const std::string& lTagName = lNode.GetName();
			const std::string& lName = lNode.GetAttribute<std::string>( "name", "" );

			CRenderableObject* p_object = NULL;
			if ( lTagName == "MeshInstance" )
			{
				if( SMeshMInstance->Exist( lNode.GetAttribute<std::string>( "core", "no_static_mesh " ) ) )
				{
					const std::string& lType = lNode.GetAttribute<std::string>( "type", "static" );
					p_object = ( lType == "static" ) ? AddStatic( lNode ) : AddDynamic( lNode );
				}
				else
				{
					LOG_ERROR_APPLICATION("Loading a instance mesh with out static mesh");
				}
			}
			else if ( lTagName == "AnimatedInstance" )
			{
				p_object = new CAnimatedInstanceModel( lNode );
			}

			if( p_object )
			{
				p_object->SetRoomName( l_RoomName );
				if ( !lRenderableObjectManager->AddResource( lName, p_object ) )
				{
					LOG_ERROR_APPLICATION( "Error adding renderable object %s!", lName.c_str() );
					CHECKED_DELETE( p_object );
				}
			}
		}

		lOk = AddResource(l_Name, lRenderableObjectManager );
		ASSERT( lOk, "The layer %s could not be added", l_Name.c_str() );
		if( !lOk ) CHECKED_DELETE( lRenderableObjectManager );
	}

	return lOk;
}

void CRenderableObjectsLayersManager::Update()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

    for ( ; itb != ite; ++itb )
        ( *itb )->Update();
}
void CRenderableObjectsLayersManager::Render()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

    for ( ; itb != ite; ++itb )
        ( *itb )->Render();
}
void CRenderableObjectsLayersManager::Render( const std::string& LayerName )
{
    TMapResources l_ResourcesMap = GetResourcesMap();
    TMapResources::iterator itb = l_ResourcesMap.find( LayerName );

    if ( itb != l_ResourcesMap.end() )
        ( *itb ).second.m_Value->Render();
}
CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager( const std::string &l_Name )
{
    return ( l_Name == "" ) ? m_DefaultRenderableObjectManager : GetResource( l_Name.c_str() );
}

CInstanceMesh* CRenderableObjectsLayersManager::AddDynamic( const CXMLTreeNode& atts )
{
	CInstanceMesh* l_InstanceMesh = new CInstanceMesh( atts );
	
	l_InstanceMesh->SetType("dynamic");

	// User data
	CPhysicUserData * lData = new CPhysicUserData( l_InstanceMesh->GetName() );
	lData->SetGroup( ECG_DYNAMIC_OBJECTS );
	
	// Phyx actor
	CPhysicActor* l_MeshActor = new CPhysicActor(lData);
	l_MeshActor->SetCollisionGroup( ECG_DYNAMIC_OBJECTS );

	// Obtain a box from the static mesh aabb
	CStaticMesh* l_StaticMesh = l_InstanceMesh->GetStaticMesh();

    Math::AABB3f l_AABB = l_StaticMesh->GetAABB();
    Math::Vect3f l_Pos = l_InstanceMesh->GetTransform() * l_AABB.GetCenter();

    l_MeshActor->AddBoxShape( Vect3f( l_AABB.GetWidth() * 0.5f, l_AABB.GetHeight() * 0.5f, l_AABB.GetDepth() * 0.5f ), l_Pos );
    l_MeshActor->CreateBody( 1.0f );

	l_InstanceMesh->SetActor( l_MeshActor );
	PhysXMInstance->AddPhysicActor( l_MeshActor );

	return l_InstanceMesh;
}

CInstanceMesh* CRenderableObjectsLayersManager::AddStatic( const CXMLTreeNode& atts )
{
	CInstanceMesh* l_InstanceMesh = new CInstanceMesh( atts );

	l_InstanceMesh->SetType("static");

	// If there is no ase loaded and we want phyx
	if (!PhysXMInstance->GetLoadASE())
    {
		CPhysicActor* l_MeshActor = new CPhysicActor( new CPhysicUserData( l_InstanceMesh->GetName() ) );
		NxTriangleMesh* l_TriangleMesh = PhysXMInstance->GetCookingMesh()->CreatePhysicMesh( l_InstanceMesh->GetVertexBuffer(), l_InstanceMesh->GetIndexBuffer() );
		l_MeshActor->AddMeshShape( l_TriangleMesh, l_InstanceMesh->GetTransform() );
		l_InstanceMesh->SetActor( l_MeshActor );
		PhysXMInstance->AddPhysicActor( l_MeshActor );
	}

	return l_InstanceMesh;
}

void CRenderableObjectsLayersManager::AddNewInstaceMesh( const CXMLTreeNode& atts, const std::string &l_Layer, const std::string &l_RoomName )
{
	if( SMeshMInstance->Exist( atts.GetAttribute<std::string>( "core", "no_static_mesh " ) ) )
	{
		const std::string& lType = atts.GetAttribute<std::string>( "type", "static" );
		CInstanceMesh* l_InstanceMesh = ( lType == "static" ) ? AddStatic( atts ) : AddDynamic( atts );

		if( l_InstanceMesh )
		{
			l_InstanceMesh->SetRoomName( l_RoomName );

			CRenderableObjectsManager* lRenderableObjectManager = GetRenderableObjectManager( l_Layer );

			if ( !lRenderableObjectManager->AddResource( l_InstanceMesh->GetName(), l_InstanceMesh ) )
			{
				LOG_ERROR_APPLICATION( "Error adding instance mesh %s!", l_InstanceMesh->GetName().c_str() );
				CPhysicUserData * lData = l_InstanceMesh->GetActor()->GetUserData();
				CHECKED_DELETE( lData );
				CHECKED_DELETE( l_InstanceMesh );
			}
		} 
	}
	else
	{
		LOG_ERROR_APPLICATION("Loading a instance mesh with out static mesh");
	}
}
