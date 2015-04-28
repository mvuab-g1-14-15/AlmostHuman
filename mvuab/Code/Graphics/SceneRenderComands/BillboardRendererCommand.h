#ifndef _BILLBOARD_RENDER_COMMAND_
#define _BILLBOARD_RENDER_COMMAND_

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CBillboardRenderCommand : public CSceneRendererCommand
{
    protected:
     
    public:
        CBillboardRenderCommand(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
};

#endif