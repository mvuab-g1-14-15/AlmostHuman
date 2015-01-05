#include "SceneRenderComands\SceneRendererCommand.h"

CSceneRendererCommand::CSceneRendererCommand(CXMLTreeNode &atts)
{
    if(atts.Exists())
    {
        SetName(atts.GetName());
        SetVisible(atts.GetBoolProperty("active", false));
    }
}
