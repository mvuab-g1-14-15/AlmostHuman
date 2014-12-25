#ifndef SCENE_RENDERER_COMMAND_
#define SCENE_RENDERER_COMMAND_
#pragma once


#include "Utils\Name.h"
#include "Utils\Visible.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CSceneRendererCommand: public CVisible, public CName
{
    public:
        CSceneRendererCommand(CXMLTreeNode &atts);
        virtual ~CSceneRendererCommand();

        virtual void Execute(CGraphicsManager &GM) = 0;
};
#endif