#include "SceneRenderComands\CaptureFrameBufferSceneRendererCommand.h"
#include "GraphicsManager.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts)
    :CStagedTexturedRendererCommand(atts)
{
}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
    
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CGraphicsManager &GM)
{

}