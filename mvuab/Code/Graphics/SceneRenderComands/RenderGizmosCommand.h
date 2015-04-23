#ifndef RENDER_GIZMOS_COMMAND_
#define RENDER_GIZMOS_COMMAND_

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CRenderGizmosCommand : public CSceneRendererCommand
{
public:
  CRenderGizmosCommand( CXMLTreeNode& atts );
  ~CRenderGizmosCommand();

  void Execute( CGraphicsManager& GM );
private:

  bool m_RenderGizmos;
};
#endif