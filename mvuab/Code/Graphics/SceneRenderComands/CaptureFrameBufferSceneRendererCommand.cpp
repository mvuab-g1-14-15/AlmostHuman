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
	for(int i = 0; i < m_StageTextures.size()  ;++i)
		m_StageTextures[i].m_Texture->CaptureFrameBuffer(m_StageTextures[i].m_StageId);
}