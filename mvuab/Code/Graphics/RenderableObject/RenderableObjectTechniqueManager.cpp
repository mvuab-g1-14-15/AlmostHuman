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
    std::string err = "ERROR reading the file " + FileName;
    MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
    exit( EXIT_FAILURE ); // TODO RAUL
  }

  m_FileName = FileName;
  CXMLTreeNode  TreeNode = l_File["renderable_object_techniques"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      std::string TagName = TreeNode( i ).GetName();

      if ( TagName == "pool_renderable_object_technique" )
      {
        CXMLTreeNode  SubTreeNode = TreeNode( i );
        int SubCount = SubTreeNode.GetNumChildren();

        for ( int j = 0; j < SubCount; ++j )
        {
          std::string SubTagName = SubTreeNode( j ).GetName();

          if ( SubTagName == "default_technique" )
          {
            std::string  l_ROTName = GetRenderableObjectTechniqueNameByVertexType( SubTreeNode(
                                       j ).GetIntProperty( "vertex_type", 0 ) );
            const std::string& l_Technique = SubTreeNode( j ).GetPszProperty( "technique", "" );

            if ( 0 == GetResource( l_Technique ) )
              InsertRenderableObjectTechnique( l_ROTName, l_Technique );
          }
        }

        CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(
          TreeNode( i ) );
        m_PoolRenderableObjectTechniques.AddResource( PoolRenderableObjectTechnique->GetName(),
            PoolRenderableObjectTechnique );
      }
    }
  }
  else
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
                                           "RenderableObjectTechniqueManager::Load->Error trying to read the file: %s", FileName.c_str() );
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
  AddResource( ROTName, l_RenderableObjectTechnique );
}

void CRenderableObjectTechniqueManager::ReLoad()
{
  Destroy();
  Load( m_FileName );
}