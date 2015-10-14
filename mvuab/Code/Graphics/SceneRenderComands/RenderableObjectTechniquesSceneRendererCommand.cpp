#include "SceneRenderComands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "GraphicsManager.h"

#include "EngineManagers.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode& atts) : CSceneRendererCommand(atts)
{
    m_PoolRenderableObjectTechnique = ROTMInstance->GetPoolRenderableObjectTechniques().GetResource(atts.GetAttribute<std::string>("pool", ""));
    ASSERT(m_PoolRenderableObjectTechnique, "Invalid Pool %s",  atts.GetAttribute<std::string>("pool", ""));
}

CRenderableObjectTechniquesSceneRendererCommand::~CRenderableObjectTechniquesSceneRendererCommand()
{
}

void CRenderableObjectTechniquesSceneRendererCommand::Execute( CGraphicsManager& GM )
{
    m_PoolRenderableObjectTechnique->Apply();
}