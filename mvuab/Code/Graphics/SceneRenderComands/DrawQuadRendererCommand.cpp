#include "SceneRenderComands\DrawQuadRendererCommand.h"


CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &atts): CStagedTexturedRendererCommand(atts)
{
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand()
{
    
}

void CDrawQuadRendererCommand::Execute(CGraphicsManager &GM)
{

}