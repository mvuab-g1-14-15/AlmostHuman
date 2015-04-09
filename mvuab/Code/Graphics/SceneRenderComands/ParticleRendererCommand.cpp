#include "SceneRenderComands\ParticleRendererCommand.h"
#include "Particles\ParticleManager.h"
#include "GraphicsManager.h"
#include "Core.h"

CParticleRenderCommand::CParticleRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CParticleRenderCommand::Execute( CGraphicsManager& GM )
{
    ParticleMInstance->Render();
}