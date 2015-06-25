#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Utils\MapManager.h"

#include "Effects\EffectManager.h"
#include "EngineConfig.h"
#include "EngineManagers.h"

#include <map>
#include <sstream>

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
    : CManager()
{
}

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager( CXMLTreeNode& atts)
    : CManager(atts)
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

void CRenderableObjectTechniqueManager::Init()
{
    Load( mConfigPath.c_str() );
}

void CRenderableObjectTechniqueManager::Load(const std::string& aFile)
{
    CXMLTreeNode TreeNode, lPoolXml;
    if ( lPoolXml.LoadAndFindNode(aFile.c_str(), "renderable_object_techniques", TreeNode ) )
    {
        for (uint32 i = 0, lCount = TreeNode.GetNumChildren(); i < lCount; ++i)
        {
            CXMLTreeNode& l_PoolNode = TreeNode(i);
            const std::string& TagName = l_PoolNode.GetName();

            if (TagName == "pool_renderable_object_technique")
            {
                CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(l_PoolNode);
                if(!m_PoolRenderableObjectTechniques.AddResource(PoolRenderableObjectTechnique->GetName(),
                        PoolRenderableObjectTechnique))
                {
                    CHECKED_DELETE(PoolRenderableObjectTechnique);
                    continue;
                }

                for (int j = 0, lPoolCount = l_PoolNode.GetNumChildren(); j < lPoolCount; ++j )
                {
                    const std::string& SubTagName = l_PoolNode(j).GetName();
                    if (SubTagName == "default_technique")
                    {
                        const  std::string&  l_VertexTypeStr = GetRenderableObjectTechniqueNameByVertexType(l_PoolNode(
                                j).GetAttribute<int32>("vertex_type", 0));
                        const std::string& l_TechniqueName = l_PoolNode(j).GetAttribute<std::string>("technique", "");

                        InsertRenderableObjectTechnique( l_VertexTypeStr , l_TechniqueName );
                        PoolRenderableObjectTechnique->AddElement(l_VertexTypeStr, l_TechniqueName, GetResource(l_VertexTypeStr));
                    }
                }


            }
            else if( TagName == "pool")
            {
                Load(l_PoolNode.GetAttribute<std::string>("file", "no_file"));
            }
        }
    }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType )
{
    std::stringstream l_VertexType;
    l_VertexType << "DefaultROTTechnique_" << VertexType;

    return l_VertexType.str();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique
(
    const std::string& aROTName,
    const std::string& aTechniqueName )
{
    // Only store the default renderable objects techniques, therefore check if they exist before inserting them
    if( !Exist(aROTName) )
    {
        CRenderableObjectTechnique* lRenderableObjectTechnique =
            new CRenderableObjectTechnique( aROTName,
                                            EffectManagerInstance->GetResource( aTechniqueName ) );

        if( !AddResource(aROTName, lRenderableObjectTechnique) )
        {
            CHECKED_DELETE(lRenderableObjectTechnique);
        }
    }
}

void CRenderableObjectTechniqueManager::ReLoad()
{
    Destroy();
    Init();
}