#include "SceneRenderComands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(
  CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
{
  CRenderableObjectTechniqueManager* l_ROTManager =
    CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager();
  const std::string& l_PoolName = atts.GetPszProperty( "pool", "" );
  m_PoolRenderableObjectTechnique = l_ROTManager->GetPoolRenderableObjectTechniques().GetResource(
                                      l_PoolName );
}

CRenderableObjectTechniquesSceneRendererCommand::~CRenderableObjectTechniquesSceneRendererCommand()
{
  m_PoolRenderableObjectTechnique->Destroy();
}

void CRenderableObjectTechniquesSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  m_PoolRenderableObjectTechnique->Apply();
}