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
//#include "NxTriangleMesh.h"

#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"


CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
    : m_DefaultRenderableObjectManager(0), CManager()
{
}

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager(CXMLTreeNode& atts)
    : m_DefaultRenderableObjectManager(0), CManager(atts)
{
}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
    Destroy();
}
void CRenderableObjectsLayersManager::Destroy()
{
    CTemplatedVectorMapManager::Destroy();
}

void CRenderableObjectsLayersManager::Init()
{
    CXMLTreeNode l_File;

    if ( !l_File.LoadFile( mConfigPath.c_str() ) )
    {
        std::string err = "ERROR reading the file " + mConfigPath;

        MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
        exit( EXIT_FAILURE );
    }

    CXMLTreeNode  TreeNode = l_File["RenderableObjects"];

    if (!TreeNode.Exists())
    {
        return;
    }

    for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
    {
        const std::string& lTagName = TreeNode( i ).GetName();
        const std::string& lName = TreeNode( i ).GetPszProperty("name", "");

        if (lTagName == "comment")
        {
            continue;
        }

        if (lTagName == "layer")
        {
            if(TreeNode( i ).GetBoolProperty("default", false))
            {
                m_DefaultRenderableObjectManager = new CRenderableObjectsManager();

                if (!AddResource( lName, m_DefaultRenderableObjectManager))
                {
                    LOG_ERROR_APPLICATION( "Error adding layer %s!", lName.c_str() );
                    CHECKED_DELETE( m_DefaultRenderableObjectManager );
                }
            }
            else
            {
                CRenderableObjectsManager* RenderableObjectManager = new CRenderableObjectsManager();

                if (!AddResource(lName, RenderableObjectManager))
                {
                    LOG_ERROR_APPLICATION( "Error adding layer %s!", lName.c_str() );
                    CHECKED_DELETE( RenderableObjectManager );
                }
            }
        }
        else
        {
            CRenderableObjectsManager* lRenderableObjectManager = GetRenderableObjectManager(TreeNode(i));
            ASSERT( lRenderableObjectManager, "Check the layer of the objects" );

            if (lTagName == "MeshInstance")
            {
                /*CInstanceMesh* l_InstanceMesh = new CInstanceMesh(TreeNode(i));

                if (!lRenderableObjectManager->AddResource(lName, l_InstanceMesh))
                {
                    LOG_ERROR_APPLICATION("Error adding instance mesh %s!", lName.c_str());
                    CHECKED_DELETE(l_InstanceMesh);
                }*/

                AddNewInstaceMesh( TreeNode(i) );
            }
            else if (lTagName == "AnimatedInstance")
            {
                CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel(TreeNode(i));

                if (!lRenderableObjectManager->AddResource(lName, l_AnimatedInstance))
                {
                    LOG_ERROR_APPLICATION("Error adding animated mesh %s!", lName.c_str());
                    CHECKED_DELETE(l_AnimatedInstance);
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
    for (; itb != ite; ++itb)
    {
        (*itb)->Update();
    }
}
void CRenderableObjectsLayersManager::Render()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();
    for (; itb != ite; ++itb)
    {
        (*itb)->Render();
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
CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager( CXMLTreeNode& Node )
{
    const std::string& l_Layer = Node.GetPszProperty("layer", "");
    return (l_Layer == "") ? m_DefaultRenderableObjectManager : GetResource(l_Layer.c_str());
}

void CRenderableObjectsLayersManager::AddNewInstaceMesh(CXMLTreeNode &atts)
{
    CInstanceMesh* l_InstanceMesh = new CInstanceMesh(atts);
    l_InstanceMesh->SetType(atts.GetPszProperty("type", "static"));
    
    bool lOk = false;
    string l_Name = l_InstanceMesh->GetName();

    const std::vector<Math::Vect3f> &l_VertexBuffer = l_InstanceMesh->GetVertexBuffer();
    const std::vector<uint32> &l_IndexBuffer = l_InstanceMesh->GetIndexBuffer();
    
    int l_VecElements = l_VertexBuffer.size();
    std::vector<Math::Vect3f> l_AuxVertexBuffer;
    l_AuxVertexBuffer.resize(l_VertexBuffer.size());

    #pragma omp parallel for
    for(int i = 0; i < l_VecElements; ++i)
    {
        l_AuxVertexBuffer[i] = l_InstanceMesh->GetTransform() * l_VertexBuffer[i];
    }

    CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData(l_Name);
    CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataASEMesh);

    NxTriangleMesh* l_TriangleMesh = PhysXMInstance->GetCookingMesh()->CreatePhysicMesh(l_AuxVertexBuffer, std::vector<uint32>(l_IndexBuffer));
    l_MeshActor->AddMeshShape(l_TriangleMesh, Math::Vect3f(0.0f, 0.0f, 0.0f));
    bool oK = false;
    
    if(PhysXMInstance->CMapManager<CPhysicActor>::GetResource(l_Name) == 0)
    {
        if(PhysXMInstance->AddPhysicActor(l_MeshActor))
        {
            PhysXMInstance->CMapManager<CPhysicActor>::AddResource(l_Name, l_MeshActor);
            oK = true;
        }
    }
    
    if(!oK)
    {
        CHECKED_DELETE(l_MeshActor);
        CHECKED_DELETE(l_pPhysicUserDataASEMesh);
    }
    
    if( l_InstanceMesh->GetType() == "static" )
    {
    }
    else if( l_InstanceMesh->GetType() == "dynamic" )
    {
        //l_MeshActor->CreateBody(0.1f);
    }
    
    CRenderableObjectsManager* lRenderableObjectManager = GetRenderableObjectManager(atts);
    if (!lRenderableObjectManager->AddResource(l_Name, l_InstanceMesh))
    {
        LOG_ERROR_APPLICATION("Error adding instance mesh %s!", l_Name.c_str());
        CHECKED_DELETE(l_InstanceMesh);
    }
}
