#include "SceneRenderComands\ParticleRendererCommand.h"
#include "Particles\ParticleSystemManager.h"
#include "GraphicsManager.h"

#include "EngineManagers.h"

CParticleRenderCommand::CParticleRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CParticleRenderCommand::Execute( CGraphicsManager& GM )
{
    PSManager->Render();
}