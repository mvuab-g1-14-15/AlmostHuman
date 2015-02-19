#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode& TreeNode) : CName(TreeNode.GetPszProperty("name", ""))
{
  std::string nameProperty = "";
  std::string l_Name = TreeNode.GetPszProperty( "name" );
  size_t count = TreeNode.GetNumChildren();

  for ( size_t i = 0; i < count; ++i )
  {
    nameProperty = TreeNode( i ).GetName();

    if ( nameProperty == "default_technique" )
    {
      size_t l_VertexType = TreeNode( i ).GetIntProperty( "vertex_type", 0 );
      std::string l_TechniqueName = CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(l_VertexType);

      std::string l_Technique = TreeNode( i ).GetPszProperty( "technique", "" );
      CRenderableObjectTechnique* l_ROT = CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetResource(l_TechniqueName);

      AddElement( l_TechniqueName, l_Technique, l_ROT );
    }
  }
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
  Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
  for ( size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i )
    CHECKED_DELETE( m_RenderableObjectTechniqueElements[i] );
  
  m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement( const std::string& Name, const std::string& TechniqueName, CRenderableObjectTechnique* ROTOnRenderableObjectTechniqueManager )
{
  CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement = new CPoolRenderableObjectTechniqueElement
  ( 
        Name, 
        CEffectManager::GetSingletonPtr()->GetResource( TechniqueName ), 
        ROTOnRenderableObjectTechniqueManager
  );

  m_RenderableObjectTechniqueElements.push_back( PoolRenderableObjectTechniqueElement );
}

void CPoolRenderableObjectTechnique::Apply()
{
  size_t l_Count = m_RenderableObjectTechniqueElements.size();

  for ( size_t i = 0; i < l_Count; ++i )
  {
    m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(
      m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique() );
  }
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement
    ( 
        const std::string& Name, CEffectTechnique* EffectTechnique,
        CRenderableObjectTechnique* OnRenderableObjectTechniqueManager
    )
{
  m_RenderableObjectTechnique = CRenderableObjectTechnique( Name, EffectTechnique );
  m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
}