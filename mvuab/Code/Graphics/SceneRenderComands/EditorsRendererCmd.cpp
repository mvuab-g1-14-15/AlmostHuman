#include "SceneRenderComands\EditorsRendererCmd.h"
#include "EditorsManager.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CEditorsRenderCmd::CEditorsRenderCmd( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CEditorsRenderCmd::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CEditorsRenderCmd::Execute", Profiler::Color::Orchid )
    CEngineManagers::GetSingletonPtr()->GetEditorsManager()->Render();
}