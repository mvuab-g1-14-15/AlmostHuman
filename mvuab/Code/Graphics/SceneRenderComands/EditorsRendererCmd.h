#ifndef _EDITORS_RENDER_COMMAND_
#define _EDITORS_RENDER_COMMAND_

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CEditorsRenderCmd : public CSceneRendererCommand
{
    protected:

    public:
        CEditorsRenderCmd(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
};

#endif