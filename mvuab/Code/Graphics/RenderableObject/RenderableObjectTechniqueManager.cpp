#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Utils\MapManager.h"
#include "Effects\EffectManager.h"

#include <map>
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
    CMapManager::Destroy();
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
                CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(TreeNode(i));

                CXMLTreeNode  SubTreeNode = TreeNode(i);
                int SubCount = SubTreeNode.GetNumChildren();

                for( int j = 0; j < SubCount; ++j )
                {
                    std::string SubTagName = SubTreeNode(j).GetName();
                    if( SubTagName == "default_technique" )
                    {
						std::string  l_ROTName = std::string("DefaultROTTechnique_")+std::string(SubTreeNode(j).GetPszProperty("vertex_type", ""));
						
						const std::string &l_Technique = SubTreeNode(j).GetPszProperty("technique", "");
						
                        PoolRenderableObjectTechnique->AddElement(l_ROTName, l_Technique, 
                            new CRenderableObjectTechnique(l_ROTName, 
                                CEffectManager::GetSingletonPtr()->GetResource(l_Technique))); 

						if(0==GetResource(l_Technique))
							InsertRenderableObjectTechnique(l_ROTName, l_Technique);
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
    std::ostringstream l_VertexType;
    l_VertexType << "DefaultROTTechnique_"<<VertexType;

    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();

    for(; itb != ite; ++itb)
    {
        if (itb->second->GetName() == l_VertexType.str())
        {
            return itb->second->GetName();
        }
    }
    return "";
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
    AddResource(TechniqueName, new CRenderableObjectTechnique(ROTName, CEffectManager::GetSingletonPtr()->GetResource(TechniqueName))); 
}
