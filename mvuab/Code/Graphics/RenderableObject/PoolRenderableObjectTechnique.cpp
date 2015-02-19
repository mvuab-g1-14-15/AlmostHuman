#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include <sstream>

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique( CXMLTreeNode& TreeNode ) 
  : CName( TreeNode.GetPszProperty( "name", "" ) )
{
  int count = TreeNode.GetNumChildren();
  for ( int j = 0; j < count; ++j )
  {
	  CXMLTreeNode & l_CurrentSubNode = TreeNode( j );
    const std::string & SubTagName = l_CurrentSubNode.GetName();

    if ( SubTagName == "default_technique" )
    {
      int l_VertexType = l_CurrentSubNode.GetIntProperty( "vertex_type", 0 );
      std::stringstream l_VertexTypeStr;
		  l_VertexTypeStr << "DefaultROTTechnique_" << l_VertexType;
      const std::string & l_TechniqueName = l_CurrentSubNode.GetPszProperty( "technique", "" );
      CRenderableObjectTechniqueManager *l_ROTM=CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager();
      CRenderableObjectTechnique* l_ROT = 
        CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetResource( l_TechniqueName );

      if(l_ROT==NULL)
      {
        l_ROT=new CRenderableObjectTechnique(l_TechniqueName, CEffectManager::GetSingletonPtr()->GetResource( l_TechniqueName));
        l_ROTM->AddResource(l_TechniqueName, l_ROT);
      }

      AddElement( l_VertexTypeStr.str(), l_TechniqueName, l_ROT );
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
  
  if(m_RenderableObjectTechniqueElements.size() != 0) m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement( const std::string& Name,
    const std::string& TechniqueName,
    CRenderableObjectTechnique* ROTOnRenderableObjectTechniqueManager )
{
  CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement =
    new CPoolRenderableObjectTechniqueElement( Name,
        CEffectManager::GetSingletonPtr()->GetResource( TechniqueName ),
        ROTOnRenderableObjectTechniqueManager );
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
( const std::string& Name, CEffectTechnique* EffectTechnique,
  CRenderableObjectTechnique* OnRenderableObjectTechniqueManager )
{
  m_RenderableObjectTechnique = CRenderableObjectTechnique( Name, EffectTechnique );
  m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
}