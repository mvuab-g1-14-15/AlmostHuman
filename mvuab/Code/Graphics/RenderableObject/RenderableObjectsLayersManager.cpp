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


CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
    : m_DefaultRenderableObjectManager(0)
    , CManager()
{
}

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager( const CXMLTreeNode& atts)
    : m_DefaultRenderableObjectManager(0)
    , CManager(atts)
{
}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
    Destroy();
    for(unsigned int i = 0; i < m_PhyscsUserData.size(); i++)
    {
        CHECKED_DELETE(m_PhyscsUserData.at(i));
    }
}
void CRenderableObjectsLayersManager::Destroy()
{
    CTemplatedVectorMapManager::Destroy();
}

void CRenderableObjectsLayersManager::Init()
{
    CXMLTreeNode l_File, TreeNode;

    if ( !l_File.LoadAndFindNode( mConfigPath.c_str(), "RenderableObjects", TreeNode ) )
    {
        FATAL_ERROR("Error loading renderable objects layer manager, with the config file %s", mConfigPath.c_str() );
    }
    else
    {
        for ( uint32 i = 0, lCount = TreeNode.GetNumChildren(); i < lCount ; ++i )
        {
            const CXMLTreeNode& lNode = TreeNode(i);
            const std::string& lTagName = lNode.GetName();
            const std::string& lName = lNode.GetAttribute<std::string>( "name", "" );

            if ( lTagName == "layer" )
            {
                if ( lNode.GetAttribute<bool>( "default", false ) )
                {
                    m_DefaultRenderableObjectManager = new CRenderableObjectsManager();

                    if ( !AddResource( lName, m_DefaultRenderableObjectManager ) )
                    {
                        LOG_ERROR_APPLICATION( "Error adding layer %s!", lName.c_str() );
                        CHECKED_DELETE( m_DefaultRenderableObjectManager );
                    }
                }
                else
                {
                    CRenderableObjectsManager* RenderableObjectManager = new CRenderableObjectsManager();

                    if ( !AddResource( lName, RenderableObjectManager ) )
                    {
                        LOG_ERROR_APPLICATION( "Error adding layer %s!", lName.c_str() );
                        CHECKED_DELETE( RenderableObjectManager );
                    }
                }
            }
            else
            {
                CRenderableObjectsManager* lRenderableObjectManager = GetRenderableObjectManager( lNode );
                ASSERT( lRenderableObjectManager, "Check the layer of the objects" );

                if ( lTagName == "MeshInstance" )
                {
                    AddNewInstaceMesh( lNode );
                }
                else if ( lTagName == "AnimatedInstance" )
                {
                    CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel( lNode );

                    if ( !lRenderableObjectManager->AddResource( lName, l_AnimatedInstance ) )
                    {
                        LOG_ERROR_APPLICATION( "Error adding animated mesh %s!", lName.c_str() );
                        CHECKED_DELETE( l_AnimatedInstance );
                    }
                }
            }
        }
    }
}
void CRenderableObjectsLayersManager::Reload()
{
    Destroy();
    Init();
}
void CRenderableObjectsLayersManager::Update()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

    for ( ; itb != ite; ++itb )
    {
        ( *itb )->Update();
    }
}
void CRenderableObjectsLayersManager::Render()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

    for ( ; itb != ite; ++itb )
    {
        ( *itb )->Render();
    }
}
void CRenderableObjectsLayersManager::Render( const std::string& LayerName )
{
    TMapResources l_ResourcesMap = GetResourcesMap();
    TMapResources::iterator itb = l_ResourcesMap.find( LayerName );

    if ( itb != l_ResourcesMap.end() )
    {
        ( *itb ).second.m_Value->Render();
    }
}
CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager( const CXMLTreeNode& Node )
{
    const std::string& l_Layer = Node.GetAttribute<std::string>( "layer", "" );
    return ( l_Layer == "" ) ? m_DefaultRenderableObjectManager : GetResource( l_Layer.c_str() );
}

void CRenderableObjectsLayersManager::AddNewInstaceMesh( const CXMLTreeNode& atts )
{
    CInstanceMesh* l_InstanceMesh = new CInstanceMesh( atts );
    l_InstanceMesh->SetType( atts.GetAttribute<std::string>( "type", "static" ) );

    bool lOk = false;
    const string& l_Name = l_InstanceMesh->GetName();

    CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( l_Name );

    if(l_InstanceMesh->GetType() == "dynamic")
    {
        l_pPhysicUserDataMesh->SetGroup( ECG_DYNAMIC_OBJECTS );
    }

    CPhysicActor* l_MeshActor = new CPhysicActor( l_pPhysicUserDataMesh );

    if(l_InstanceMesh->GetType() == "dynamic")
    {
        CStaticMesh* l_StaticMesh = l_InstanceMesh->GetStaticMesh();

        Math::AABB3f l_AABB = l_StaticMesh->GetAABB();
        Math::Vect3f l_Pos = l_InstanceMesh->GetTransform() * l_AABB.GetCenter();

        l_MeshActor->AddBoxShape(Vect3f( l_AABB.GetWidth() * 0.5f, l_AABB.GetHeight() * 0.5f, l_AABB.GetDepth() * 0.5f), l_Pos);
        l_MeshActor->CreateBody(1.0f);
    }
    else
    {
        NxTriangleMesh* l_TriangleMesh = PhysXMInstance->GetCookingMesh()->CreatePhysicMesh( l_InstanceMesh->GetVertexBuffer(),
                                         l_InstanceMesh->GetIndexBuffer() );
        l_MeshActor->AddMeshShape(l_TriangleMesh, l_InstanceMesh->GetTransform() );
    }

    if(PhysXMInstance->CMapManager<CPhysicActor>::GetResource(l_Name) == 0 && PhysXMInstance->AddPhysicActor(l_MeshActor) &&
            PhysXMInstance->CMapManager<CPhysicActor>::AddResource(l_Name, l_MeshActor))
    {
        lOk = true;
        m_PhyscsUserData.push_back(l_pPhysicUserDataMesh);
    }

    if(!lOk)
    {
        PhysXMInstance->ReleasePhysicActor( l_MeshActor );

        CHECKED_DELETE(l_MeshActor);
        CHECKED_DELETE(l_pPhysicUserDataMesh);
        CHECKED_DELETE(l_pPhysicUserDataMesh);
    }
    else if ( l_InstanceMesh->GetType() == "dynamic" )
    {
        l_MeshActor->SetCollisionGroup( ECG_DYNAMIC_OBJECTS );
        l_InstanceMesh->SetActor( l_MeshActor );
    }
    else
    {
        l_InstanceMesh->SetActor( l_MeshActor );
    }

    CRenderableObjectsManager* lRenderableObjectManager = GetRenderableObjectManager( atts );

    if ( !lRenderableObjectManager->AddResource( l_Name, l_InstanceMesh ) )
    {
        LOG_ERROR_APPLICATION( "Error adding instance mesh %s!", l_Name.c_str() );
        CHECKED_DELETE( l_InstanceMesh );
    }
}
