#include "SceneRenderComands\EditorsRendererCmd.h"
#include "EditorsManager.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CEditorsRenderCmd::CEditorsRenderCmd( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CEditorsRenderCmd::Execute( CGraphicsManager& GM )
{
    CEngineManagers::GetSingletonPtr()->GetEditorsManager()->Render();
}