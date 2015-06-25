#include "SceneRenderComands\RenderGUISceneRendererCommand.h"
#include "GUIManager.h"
#include "EngineManagers.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
    , m_RenderGui( atts.GetAttribute<bool>("render", false) )
{

}

CRenderGUISceneRendererCommand::~CRenderGUISceneRendererCommand()
{

}

void CRenderGUISceneRendererCommand::Execute(CGraphicsManager &GM)
{
    if ( m_RenderGui )
    {
        CGUIManager* l_GUI = GUIInstance;
        l_GUI->Render();
    }
}