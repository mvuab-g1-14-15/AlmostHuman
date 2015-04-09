#include "AnimatedModels\AnimatedInstanceModel.h"
#include "RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager() : m_DefaultRenderableObjectManager(0)
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

void CRenderableObjectsLayersManager::Load( const std::string& FileName )
{
    m_FileName = FileName;
    CXMLTreeNode l_File;
    
    if (!l_File.LoadFile(m_FileName.c_str()))
    {
        std::string err = "ERROR reading the file " + FileName;
        MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
        exit( EXIT_FAILURE );
    }
    
    CXMLTreeNode  TreeNode = l_File["RenderableObjects"];
    if (!TreeNode.Exists()) return;

    for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
    {
        const std::string& lTagName = TreeNode( i ).GetName();
        const std::string& lName = TreeNode( i ).GetPszProperty("name", "");
        
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
    Load( m_FileName );
}
void CRenderableObjectsLayersManager::Update()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();
    for (; itb != ite; ++itb) (*itb)->Update();
}
void CRenderableObjectsLayersManager::Render()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();
    for (; itb != ite; ++itb) (*itb)->Render();
}
void CRenderableObjectsLayersManager::Render( const std::string& LayerName )
{
    TMapResources l_ResourcesMap = GetResourcesMap();
    TMapResources::iterator itb = l_ResourcesMap.find( LayerName );
    if ( itb != l_ResourcesMap.end() ) ( *itb ).second.m_Value->Render();
}
CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager( CXMLTreeNode& Node )
{
    const std::string& l_Layer = Node.GetPszProperty("layer", "");
    return (l_Layer == "") ? m_DefaultRenderableObjectManager : GetResource(l_Layer.c_str());
}