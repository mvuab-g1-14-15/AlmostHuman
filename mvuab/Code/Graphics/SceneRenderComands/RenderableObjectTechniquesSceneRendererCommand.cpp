#include "SceneRenderComands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(
  CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
  CRenderableObjectTechniqueManager* l_ROTManager =
    CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager();
  m_PoolRenderableObjectTechnique = l_ROTManager->GetPoolRenderableObjectTechniques().GetResource(
                                      atts.GetPszProperty( "pool", "" ) );
}

CRenderableObjectTechniquesSceneRendererCommand::~CRenderableObjectTechniquesSceneRendererCommand()
{
}

void CRenderableObjectTechniquesSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  m_PoolRenderableObjectTechnique->Apply();
}