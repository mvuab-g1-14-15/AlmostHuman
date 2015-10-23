#include "SceneRenderComands/RenderGizmosCommand.h"
#include "GraphicsManager.h"
#include "Gizmos/GizmosManager.h"

#include "EngineManagers.h"

CRenderGizmosCommand::CRenderGizmosCommand( CXMLTreeNode& atts )
    : CSceneRendererCommand( atts )
    , m_RenderGizmos( atts.GetAttribute<bool>( "render", false ) )
{
}

CRenderGizmosCommand::~CRenderGizmosCommand()
{

}

void CRenderGizmosCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CRenderDebugShadowMapsSceneRendererCommand::Execute", Profiler::Color::Orchid )
    if ( m_RenderGizmos )
    {
        CGizmosManager* l_GM = GizmosMInstance;
        l_GM->Render();
    }
}