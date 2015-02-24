#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "d3dx9.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(
  CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
{
  m_NameTechnique = atts.GetPszProperty( "technique", "" );
  Math::Vect3f l_Weight3 = atts.GetVect3fProperty( "weight3", Math::Vect3f( 0, 0, 0 ) );
  Math::Vect2f l_Weight2 = atts.GetVect2fProperty( "weight2", Math::Vect2f( 0, 0 ) );
  Math::Vect3f l_Offset3 = atts.GetVect3fProperty( "offset3", Math::Vect3f( 0, 0, 0 ) );
  Math::Vect2f l_Offset2 = atts.GetVect2fProperty( "offset2", Math::Vect2f( 0, 0 ) );
  m_Weight[0] = l_Weight3.x;
  m_Weight[1] = l_Weight3.y;
  m_Weight[2] = l_Weight3.z;
  m_Weight[3] = l_Weight2.x;
  m_Weight[4] = l_Weight2.y;
  m_Offset[0] = l_Offset3.x;
  m_Offset[1] = l_Offset3.y;
  m_Offset[2] = l_Offset3.z;
  m_Offset[3] = l_Offset2.x;
  m_Offset[4] = l_Offset2.y;
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  CEffectTechnique* l_EffectTechnique = CEffectManager::GetSingletonPtr()->GetResource(
                                          m_NameTechnique );
  l_EffectTechnique->SetWeight( m_Weight );
  l_EffectTechnique->SetOffset( m_Offset );
}