#include "SceneRenderComands/RenderGizmosCommand.h"
#include "GraphicsManager.h"
#include "Gizmos/GizmosManager.h"
#include "Core.h"

CRenderGizmosCommand::CRenderGizmosCommand( CXMLTreeNode& atts )
  : CSceneRendererCommand( atts )
  , m_RenderGizmos( atts.GetBoolProperty( "render", false ) )
{
}

CRenderGizmosCommand::~CRenderGizmosCommand()
{

}

void CRenderGizmosCommand::Execute( CGraphicsManager& GM )
{
  if ( m_RenderGizmos )
  {
    CGizmosManager* l_GM = CCore::GetSingletonPtr()->GetGizmosManager();
    l_GM->Render();
  }
}