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
				CXMLTreeNode l_NodePoolRenderableObjectTechnique = CXMLTreeNode::CXMLTreeNode();
				l_NodePoolRenderableObjectTechnique.WritePszProperty("name", TreeNode(i).GetPszProperty("name", ""));

                CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(l_NodePoolRenderableObjectTechnique);

                CXMLTreeNode  SubTreeNode = TreeNode(i);
                int SubCount = SubTreeNode.GetNumChildren();

                for( int j = 0; j < SubCount; ++j )
                {
                    std::string SubTagName = SubTreeNode(j).GetName();
                    if( SubTagName == "default_technique" )
                    {
						std::string l_ROTName = std::string("DefaultROTTechnique_")+std::string("%u",SubTreeNode(j).GetIntProperty("vertex_type", 0));
						
						CXMLTreeNode l_NodeEffectTechnique = CXMLTreeNode::CXMLTreeNode();
						l_NodeEffectTechnique.WritePszProperty("name", SubTreeNode(j).GetPszProperty("technique", ""));

                        PoolRenderableObjectTechnique->AddElement(l_ROTName, SubTreeNode(j).GetPszProperty("technique", ""), 
                            new CRenderableObjectTechnique(l_ROTName, 
                                new CEffectTechnique(l_NodeEffectTechnique))); 

                        InsertRenderableObjectTechnique(l_ROTName, SubTreeNode(j).GetPszProperty("technique", ""));
                    }
                }
                m_PoolRenderableObjectTechniques.AddResource(TreeNode(i).GetPszProperty("name", ""), 
					PoolRenderableObjectTechnique);
                
            }
        }
    }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType)
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
    for(; itb != ite; ++itb)
    {
        if (itb->second->GetName() == (std::string("DefaultROTTechnique_")+std::string("%u",VertexType)))
        {
            return itb->second->GetName();
        }
    }
    return "";
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
	CXMLTreeNode l_NodeEffectTechnique = CXMLTreeNode::CXMLTreeNode();
	l_NodeEffectTechnique.WritePszProperty("name", TechniqueName.c_str());

    AddResource(TechniqueName, new CRenderableObjectTechnique(ROTName, new CEffectTechnique(l_NodeEffectTechnique))); 
}
