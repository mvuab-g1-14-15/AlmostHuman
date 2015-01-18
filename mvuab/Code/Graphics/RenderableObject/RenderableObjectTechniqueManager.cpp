#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include <map>
#include "Utils\MapManager.h"
#include <sstream>


CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{

}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
    Destroy();
}

void CRenderableObjectTechniqueManager::Destroy()
{
    m_PoolRenderableObjectTechniques.Destroy();
}

void CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{
    CXMLTreeNode l_File;
    if (!l_File.LoadFile(FileName.c_str()))
    {
        std::string err = "ERROR reading the file " + FileName;

        MessageBox(NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK);
        exit(EXIT_FAILURE);
    }

    CXMLTreeNode  TreeNode =l_File["renderable_object_techniques"];
    if(TreeNode.Exists())
    {
        int count = TreeNode.GetNumChildren();
        for( int i = 0; i < count; ++i )
        {
            std::string TagName = TreeNode(i).GetName();
            if( TagName == "pool_renderable_object_technique" )
            {
                CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(CXMLTreeNode::CXMLTreeNode());
                CXMLTreeNode  SubTreeNode = TreeNode(i);
                int SubCount = SubTreeNode.GetNumChildren();
                for( int j = 0; j < SubCount; ++j )
                {
                    std::string SubTagName = SubTreeNode(j).GetName();
                    if( SubTagName == "default_technique" )
                    {
                        PoolRenderableObjectTechnique->AddElement(SubTagName, SubTreeNode(j).GetPszProperty("technique", ""), 
                            new CRenderableObjectTechnique(SubTreeNode(j).GetPszProperty("technique", ""), 
                                new CEffectTechnique(CXMLTreeNode::CXMLTreeNode()), SubTreeNode(j).GetIntProperty("vertex_type", 0))); 
                        InsertRenderableObjectTechnique(SubTagName, SubTreeNode(j).GetPszProperty("technique", ""), SubTreeNode(j).GetIntProperty("vertex_type", 0));
                    }
                }
                m_PoolRenderableObjectTechniques.AddResource(TreeNode(i).GetPszProperty("name", ""), PoolRenderableObjectTechnique);
                
            }
        }
    }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType)
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
    for(; itb != ite; ++itb)
    {
        if (itb->second->GetVertexType() == VertexType)
        {
            std::stringstream VertexTypeName;
            VertexTypeName << "DefaultROTTechnique_" << VertexType;
            return VertexTypeName.str();
        }
    }
    return "";
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName, uint32 VertexType)
{
    AddResource(ROTName, new CRenderableObjectTechnique(TechniqueName, new CEffectTechnique(CXMLTreeNode::CXMLTreeNode() ), VertexType)); 
}