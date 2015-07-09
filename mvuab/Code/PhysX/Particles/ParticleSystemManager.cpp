#include "ParticleSystemManager.h"
#include "EngineConfig.h"


CParticleSystemManager::CParticleSystemManager() : CManager()
{
}

CParticleSystemManager::CParticleSystemManager( CXMLTreeNode& atts ) : CManager( atts )
{
}

CParticleSystemManager::~CParticleSystemManager()
{
  Destroy();
}

void CParticleSystemManager::Init()
{

}

void CParticleSystemManager::Update()
{

}

void CParticleSystemManager::Render()
{

}

void CParticleSystemManager::Refresh()
{
  Destroy();
  Init();
}