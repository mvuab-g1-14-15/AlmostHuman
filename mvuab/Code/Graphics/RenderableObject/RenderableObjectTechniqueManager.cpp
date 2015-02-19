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
      const std::string & TagName = TreeNode( i ).GetName();

      if ( TagName == "pool_renderable_object_technique" )
      {
        /*CEffectManager::GetSingletonPtr()->
        CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);*/
        //GetResource(


        CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique
          = new CPoolRenderableObjectTechnique( TreeNode( i ) );

        m_PoolRenderableObjectTechniques.AddResource( PoolRenderableObjectTechnique->GetName(),
            PoolRenderableObjectTechnique );
      }
    }
  }
  else
  {
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
                                           "RenderableObjectTechniqueManager::Load->Error trying to read the file: %s", FileName.c_str() );
  }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType( uint32 VertexType )
{
  std::stringstream l_VertexType;
  l_VertexType << "DefaultROTTechnique_" << VertexType;
  return l_VertexType.str();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique( const std::string& ROTName, const std::string& TechniqueName )
{
  CRenderableObjectTechnique* l_RenderableObjectTechnique = new CRenderableObjectTechnique( ROTName, CEffectManager::GetSingletonPtr()->GetResource( TechniqueName ) );
  if(!AddResource(ROTName, l_RenderableObjectTechnique))
      CHECKED_DELETE(l_RenderableObjectTechnique);
}

void CRenderableObjectTechniqueManager::ReLoad()
{
  Destroy();
  Load( m_FileName );
}