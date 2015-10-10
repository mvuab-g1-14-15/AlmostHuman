#include "Particles\InstanceParticle.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleSystemCore.h"
#include "GraphicsManager.h"

CParticleInstance::CParticleInstance( const CXMLTreeNode& atts )
    : CObject3D( atts )
	, mActive( atts.GetAttribute<bool>( "active", false )  )
{
}

CParticleInstance::~CParticleInstance()
{
}