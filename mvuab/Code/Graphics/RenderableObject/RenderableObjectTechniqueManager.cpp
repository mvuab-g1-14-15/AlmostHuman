#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Utils\MapManager.h"
#include "Core.h"
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
	:CManager(atts)
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
  CXMLTreeNode l_File;
  //mConfigPath = EngineConfigInstance->GetRenderableObjectTechniquePath();
  if ( !l_File.LoadFile( mConfigPath.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "ERROR reading the file %s", mConfigPath.c_str() );
    return;
  }

  CXMLTreeNode  TreeNode = l_File["renderable_object_techniques"];

    if (!TreeNode.Exists())
    {
        LOG_ERROR_APPLICATION( "ERROR reading tag renderable_object_techniques\n");
        return;
    }

    for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
    {
        CXMLTreeNode& l_PoolNode = TreeNode(i);
        const std::string& TagName = l_PoolNode.GetName();
        
        if (TagName == "pool_renderable_object_technique")
        {
            CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(l_PoolNode);
            for (int j = 0; j < l_PoolNode.GetNumChildren(); ++j )
            {
                const std::string& SubTagName = l_PoolNode(j).GetName();
                if (SubTagName == "default_technique")
                {
                    const  std::string&  l_VertexTypeStr = GetRenderableObjectTechniqueNameByVertexType(l_PoolNode(j).GetIntProperty("vertex_type", 0));
                    const std::string& l_TechniqueName = l_PoolNode(j).GetPszProperty("technique", "");
                    
                    InsertRenderableObjectTechnique( l_VertexTypeStr , l_TechniqueName );
                    PoolRenderableObjectTechnique->AddElement(l_VertexTypeStr, l_TechniqueName, GetResource(l_VertexTypeStr));
                }
            }
            
            m_PoolRenderableObjectTechniques.AddResource(PoolRenderableObjectTechnique->GetName(), PoolRenderableObjectTechnique);
        }
    }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType )
{
    std::stringstream l_VertexType;
    l_VertexType << "DefaultROTTechnique_" << VertexType;
    
    return l_VertexType.str();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique( const std::string& ROTName, const std::string& TechniqueName )
{
  CRenderableObjectTechnique* l_RenderableObjectTechnique = new CRenderableObjectTechnique( ROTName,
      EffectManagerInstance->GetResource( TechniqueName ) );

  if ( !AddResource( ROTName, l_RenderableObjectTechnique ) )
    CHECKED_DELETE( l_RenderableObjectTechnique );
}

void CRenderableObjectTechniqueManager::ReLoad()
{
  Destroy();
  Init();
}