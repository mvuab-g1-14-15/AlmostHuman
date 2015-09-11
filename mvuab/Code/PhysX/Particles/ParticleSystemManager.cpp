#include "ParticleSystemManager.h"
#include "EngineConfig.h"
#include "Emitters/EmitterFactory.h"


CParticleSystemManager::CParticleSystemManager()
    : CManager()
    , mEmitterFactory( new CEmitterFactory() )
{
}

CParticleSystemManager::CParticleSystemManager( CXMLTreeNode& atts )
    : CManager( atts )
    , mEmitterFactory( new CEmitterFactory() )
{
}

CParticleSystemManager::~CParticleSystemManager()
{
    Destroy();
    CHECKED_DELETE( mEmitterFactory );
}

void CParticleSystemManager::Refresh()
{
  Destroy();
  LoadXML();
}

void CParticleSystemManager::LoadXML()
{
  // Parse all the particle systems
    CXMLTreeNode l_XML, l_Node;

    if ( l_XML.LoadAndFindNode( mConfigPath.c_str(), "particles_systems", l_Node ) )
    {
        for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lCurrentSystem = l_Node( i );
            const std::string& lSystemCoreName = lCurrentSystem.GetAttribute<std::string>( "name", "" );

            CParticleSystemCore* lSystemCore = new CParticleSystemCore( lCurrentSystem, mEmitterFactory );

            ASSERT( lSystemCore, "Null SystemCore" );

            if ( lSystemCoreName == "" || !AddResource( lSystemCoreName, lSystemCore ) )
            {
                LOG_ERROR_APPLICATION( "Error initing emitter %s", lSystemCoreName );
                CHECKED_DELETE( lSystemCore );
            }
        }
    }
}

void CParticleSystemManager::Init()
{
    // Init the factory
    mEmitterFactory->InitFactory();
    LoadXML();
}

void CParticleSystemManager::Update()
{
    for( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end();
            lItb != lIte; ++lItb )
    {
        (*lItb)->Update();
    }
}

