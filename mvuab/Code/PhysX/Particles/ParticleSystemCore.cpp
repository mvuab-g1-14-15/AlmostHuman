#include "ParticleSystemCore.h"
#include "EngineConfig.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"


CParticleSystemCore::CParticleSystemCore()
{
}

CParticleSystemCore::CParticleSystemCore( CXMLTreeNode& atts )
{
}

CParticleSystemCore::~CParticleSystemCore()
{
  Destroy();
}

void CParticleSystemCore::Init()
{

}

void CParticleSystemCore::Update()
{

}

void CParticleSystemCore::Render()
{

}

void CParticleSystemCore::Refresh()
{
  Destroy();
  Init();
}