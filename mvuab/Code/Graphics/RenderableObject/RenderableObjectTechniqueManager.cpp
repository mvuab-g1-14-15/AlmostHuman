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

void CRenderableObjectTechniqueManager::Load( const std::string& FileName )
{
  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( FileName.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "ERROR reading the file %s", FileName.c_str() );
    return;
  }

  m_FileName = FileName;
  CXMLTreeNode  TreeNode = l_File["renderable_object_techniques"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      CXMLTreeNode& l_PoolNode = TreeNode( i );
      const std::string& TagName = l_PoolNode.GetName();

      if ( TagName == "pool_renderable_object_technique" )
      {
        CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique
          = new CPoolRenderableObjectTechnique( l_PoolNode );
        int count = l_PoolNode.GetNumChildren();

        for ( int j = 0; j < count; ++j )
        {
          const std::string& SubTagName = l_PoolNode( j ).GetName();

          if ( SubTagName == "default_technique" )
          {
            const  std::string&  l_VertexTypeStr = GetRenderableObjectTechniqueNameByVertexType( l_PoolNode(
                j ).GetIntProperty( "vertex_type", 0 ) );
            const std::string& l_TechniqueName = l_PoolNode( j ).GetPszProperty( "technique", "" );
            InsertRenderableObjectTechnique( l_VertexTypeStr , l_TechniqueName );
            PoolRenderableObjectTechnique->AddElement( l_VertexTypeStr, l_TechniqueName,
                GetResource( l_VertexTypeStr ) );
          }
        }

        m_PoolRenderableObjectTechniques.AddResource( PoolRenderableObjectTechnique->GetName(),
            PoolRenderableObjectTechnique );
      }
    }
  }
  else
  {
    LOG_ERROR_APPLICATION(
                                           "RenderableObjectTechniqueManager::Load->Error trying to read the file: %s", FileName.c_str() );
  }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(
  uint32 VertexType )
{
  std::stringstream l_VertexType;
  l_VertexType << "DefaultROTTechnique_" << VertexType;
  return l_VertexType.str();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique( const std::string& ROTName,
    const std::string& TechniqueName )
{
  CRenderableObjectTechnique* l_RenderableObjectTechnique = new CRenderableObjectTechnique( ROTName,
      CEffectManager::GetSingletonPtr()->GetResource( TechniqueName ) );

  if ( !AddResource( ROTName, l_RenderableObjectTechnique ) )
    CHECKED_DELETE( l_RenderableObjectTechnique );
}

void CRenderableObjectTechniqueManager::ReLoad()
{
  Destroy();
  Load( m_FileName );
}