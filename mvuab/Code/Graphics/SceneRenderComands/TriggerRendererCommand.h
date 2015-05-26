#ifndef _TRIGGER_RENDER_COMMAND_
#define _TRIGGER_RENDER_COMMAND_

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CTriggerRenderCommand : public CSceneRendererCommand
{
    protected:
     
    public:
        CTriggerRenderCommand(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
};

#endif // _TRIGGER_RENDER_COMMAND_