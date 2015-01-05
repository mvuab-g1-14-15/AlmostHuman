#include "SceneRenderComands\ClearSceneRendererCommand.h"


CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &atts):CSceneRendererCommand(atts)
{
    if(atts.Exists())
    {
        m_Color = atts.GetBoolProperty("color", false);
        m_Depht = atts.GetBoolProperty("depth", false);
        m_Stencil = atts.GetBoolProperty("stencil", false);
    }
}