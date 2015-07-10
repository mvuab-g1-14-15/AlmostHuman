#include "ParticleManager.h"
#include "Timer\Timer.h"
#include "EngineConfig.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"


CParticleManager::CParticleManager() : CManager()
{
}

CParticleManager::CParticleManager( CXMLTreeNode& atts ) : CManager( atts )
{
}

CParticleManager::~CParticleManager()
{
  Destroy();
}

void CParticleManager::Init()
{
  CXMLTreeNode l_XML, l_Node;

  if ( l_XML.LoadAndFindNode( mConfigPath.c_str(), "emitters", l_Node ) )
  {
    for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
    {
      const CXMLTreeNode& lCurrentParticle = l_Node( i );
      const std::string& lNameParticle = lCurrentParticle.GetAttribute<std::string>( "name", "" );

      CInstanceParticle* lInstanceParticle = new CInstanceParticle( lCurrentParticle );

      ASSERT( lInstanceParticle, "Null Instance Particle" );

      if ( lNameParticle == "" || ( !AddResource( lNameParticle, lInstanceParticle ) ) )
      {
        LOG_ERROR_APPLICATION( "Error initing emitter %s", lNameParticle );
        CHECKED_DELETE( lInstanceParticle );
      }
    }
  }
}

void CParticleManager::Update()
{
  for ( uint32 i = 0, lEmitters = GetResourcesCount(); i < lEmitters; ++i )
  {
    CInstanceParticle* lParticleInstance = GetResourceById( i );

    lParticleInstance->Update();
  }
}

void CParticleManager::Render()
{
  for ( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i )
  {
    CInstanceParticle* lParticleInstance = GetResourceById( i );

    lParticleInstance->Render();
  }
}

void CParticleManager::Refresh()
{
  Destroy();
  Init();
}