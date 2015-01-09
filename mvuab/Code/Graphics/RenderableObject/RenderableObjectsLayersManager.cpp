#include "RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
{

}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{

}
void CRenderableObjectsLayersManager::Destroy()
{
    Destroy();
}
void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
    m_FileName = FileName;
    CXMLTreeNode l_File;
    if (!l_File.LoadFile(m_FileName.c_str()))
    {
        std::string err = "ERROR reading the file " + FileName;

        MessageBox(NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK);
        exit(EXIT_FAILURE);
    }

    CXMLTreeNode  TreeNode =l_File["renderable_objects"];
    if(TreeNode.Exists())
    {
        int count = TreeNode.GetNumChildren();
        for( int i = 0; i < count; ++i )
        {
            std::string TagName = TreeNode(i).GetName();
            if( TagName == "layer" && TreeNode(i).GetBoolProperty("default", false))
            {
                m_DefaultRenderableObjectManager = new CRenderableObjectsManager();
                AddResource(TreeNode(i).GetPszProperty("name", ""), m_DefaultRenderableObjectManager);
            }
            if( TagName == "layer" && !TreeNode(i).GetBoolProperty("default", false))
            {
                CRenderableObjectsManager *RenderableObjectManager = new CRenderableObjectsManager();
                AddResource(TreeNode(i).GetPszProperty("name", ""), RenderableObjectManager);
            }
            if( TagName == "instance_mesh" && 0!=strcmp("",TreeNode(i).GetPszProperty("layer", "")))
            {
                CRenderableObjectsManager *RenderableObjectManager = GetResource(TreeNode(i).GetPszProperty("layer", ""));
                std::string l_Name=TreeNode(i).GetPszProperty("name","");
                std::string l_Core=TreeNode(i).GetPszProperty("core_name","");
                Math::Vect3f l_Pos=TreeNode(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
                float32 l_Yaw=TreeNode(i).GetFloatProperty("yaw",0.0f);
                float32 l_Pitch=TreeNode(i).GetFloatProperty("pitch",0.0f);
                float32 l_Roll=TreeNode(i).GetFloatProperty("roll",0.0f);
                Math::Vect3f l_Scale=TreeNode(i).GetVect3fProperty("scale",Math::Vect3f(1.0f,1.0f,1.0f));
                /*bool l_Visible = TreeNode(i).GetBoolProperty("visible", false);
                bool l_CreatePhysics = TreeNode(i).GetBoolProperty("create_physics", false);
                std::string l_Physics = TreeNode(i).GetPszProperty("physics_type", "");*/
                CInstanceMesh* l_InstanceMesh = new CInstanceMesh(l_Name, l_Core);
                l_InstanceMesh->SetPosition(l_Pos);
                l_InstanceMesh->SetYaw( Math::Utils::Deg2Rad(l_Yaw));
                l_InstanceMesh->SetPitch( Math::Utils::Deg2Rad(l_Pitch));
                l_InstanceMesh->SetRoll(Math::Utils::Deg2Rad(l_Roll));
                l_InstanceMesh->SetScale(l_Scale);
                RenderableObjectManager->AddResource(l_Name,l_InstanceMesh);                                
            }
            if( TagName == "instance_mesh" && 0==strcmp("",TreeNode(i).GetPszProperty("layer", "")))
            {
                std::string l_Name=TreeNode(i).GetPszProperty("name","");
                std::string l_Core=TreeNode(i).GetPszProperty("core_name","");
                Math::Vect3f l_Pos=TreeNode(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
                float32 l_Yaw=TreeNode(i).GetFloatProperty("yaw",0.0f);
                float32 l_Pitch=TreeNode(i).GetFloatProperty("pitch",0.0f);
                float32 l_Roll=TreeNode(i).GetFloatProperty("roll",0.0f);
                Math::Vect3f l_Scale=TreeNode(i).GetVect3fProperty("scale",Math::Vect3f(1.0f,1.0f,1.0f));
                /*bool l_Visible = TreeNode(i).GetBoolProperty("visible", false);
                bool l_CreatePhysics = TreeNode(i).GetBoolProperty("create_physics", false);
                std::string l_Physics = TreeNode(i).GetPszProperty("physics_type", "");*/
                CInstanceMesh* l_InstanceMesh = new CInstanceMesh(l_Name, l_Core);
                l_InstanceMesh->SetPosition(l_Pos);
                l_InstanceMesh->SetYaw( Math::Utils::Deg2Rad(l_Yaw));
                l_InstanceMesh->SetPitch( Math::Utils::Deg2Rad(l_Pitch));
                l_InstanceMesh->SetRoll(Math::Utils::Deg2Rad(l_Roll));
                l_InstanceMesh->SetScale(l_Scale);
                m_DefaultRenderableObjectManager->AddResource(l_Name,l_InstanceMesh);      
            }
        }
    }
}
void CRenderableObjectsLayersManager::Reload()
{
    Load(m_FileName);
}
void CRenderableObjectsLayersManager::Update()
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();
    for(; itb != ite; ++itb)
    {
        (*itb)->Update();
    }
}
void CRenderableObjectsLayersManager::Render(CGraphicsManager *GM)
{
    std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();
    for(; itb != ite; ++itb)
    {
        (*itb)->Render();
    }
}
void CRenderableObjectsLayersManager::Render(CGraphicsManager *GM, const std::string &LayerName)
{
    std::map<std::string, CMapResourceValue> l_ResourcesMap = GetResourcesMap();
    std::map<std::string, CMapResourceValue>::iterator itb = l_ResourcesMap.find(LayerName);
    if(itb != l_ResourcesMap.end())
        (*itb).second.m_Value->Render();
}
CRenderableObjectsManager * CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node)
{
    return m_DefaultRenderableObjectManager;
}