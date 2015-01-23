#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "GraphicsManager.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &atts): CStagedTexturedRendererCommand(atts)
{
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand()
{
    
}

void CDrawQuadRendererCommand::Execute(CGraphicsManager &GM)
{
	
}