#include "AnimatedModels\AnimatedInstanceModel.h"
#include "RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "EngineConfig.h"
#include "Utils/Defines.h"
#include "EngineManagers.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "CookingMesh\PhysicCookingMesh.h"

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
                CInstanceMesh* l_InstanceMesh = new CInstanceMesh(TreeNode(i));

                if (!lRenderableObjectManager->AddResource(lName, l_InstanceMesh))
                {
                    LOG_ERROR_APPLICATION("Error adding instance mesh %s!", lName.c_str());
                    CHECKED_DELETE(l_InstanceMesh);
                }

                //AddNewInstaceMesh( TreeNode(i) );
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

void CRenderableObjectsLayersManager::AddNewInstaceMesh( const CXMLTreeNode& )
{
    /*
    const std::string& lName = atts.GetPszProperty("name", "");
    const std::string& lCoreName = atts.GetPszProperty( "core", "unknown" );
    ASSERT( lCoreName != "", "Null core name %s", lName.c_str() );

    CInstanceMesh* l_InstanceMesh = new CInstanceMesh(lName);

    CStaticMesh* lStaticMesh = SMeshMInstance->GetResource(lCoreName);
    ASSERT( lStaticMesh, "Null static mesht %s", lCoreName.c_str() );

    if( lStaticMesh )
    {
        bool lOk = false;
        // Transformo IB y VB a la posicion de la instance mesh
        if( lStaticMesh->GetType() == "static" )
        {
            // Add to scenary
            push_back al sceneario de IB y VB de la static mesh
            lOk = true;
        }
        else if( lStaticMesh->GetType() == "dynamic" )
        {
            // Rigid body
            // Añadir el actor a la instance mesh

            /*
            // Calculate the single cooking mesh
            CPhysicCookingMesh* lCookingMesh = PhysXMInstance->GetCookingMesh();
            lCookingMesh->CreatePhysicMesh(l_itBegin->first, l_itBegin->second->GetVB(), l_itBegin->second->GetIB());
            lOk = true;
          
        }
    }

    if (!lRenderableObjectManager->AddResource(lName, l_InstanceMesh))
                {
                    LOG_ERROR_APPLICATION("Error adding instance mesh %s!", lName.c_str());
                    CHECKED_DELETE(l_InstanceMesh);
                }*/
}