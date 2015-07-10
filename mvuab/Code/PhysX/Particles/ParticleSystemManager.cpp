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
  CXMLTreeNode l_XML, l_Node;

  if ( l_XML.LoadAndFindNode( mConfigPath.c_str(), "particle_systems", l_Node ) )
  {
    for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
    {
      const CXMLTreeNode& lCurrentSystem = l_Node( i );
      const std::string& lSystemCoreName = lCurrentSystem.GetAttribute<std::string>( "name", "" );

      CParticleSystemCore* lSystemCore = new CParticleSystemCore( lCurrentSystem );

      ASSERT( lSystemCore, "Null SystemCore" );

      if ( lSystemCoreName == "" || !AddResource( lSystemCoreName, lSystemCore ) )
      {
        LOG_ERROR_APPLICATION( "Error initing emitter %s", lSystemCoreName );
        CHECKED_DELETE( lSystemCore );
      }
    }
  }
}