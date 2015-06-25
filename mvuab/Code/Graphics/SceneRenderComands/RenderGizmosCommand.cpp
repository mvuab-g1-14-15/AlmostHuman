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
    if ( m_RenderGizmos )
    {
        CGizmosManager* l_GM = GizmosMInstance;
        l_GM->Render();
    }
}