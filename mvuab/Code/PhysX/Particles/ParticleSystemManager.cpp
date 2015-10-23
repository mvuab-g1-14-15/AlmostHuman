#include "ParticleSystemManager.h"
#include "EngineConfig.h"
#include "Emitters/EmitterFactory.h"
#include <boost/foreach.hpp>
#include "InstanceParticle.h"


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
  LoadInstances("Data/sala1/particles.xml"); // The refresh only is available in developer mode
}

void CParticleSystemManager::LoadXML()
{
  // Parse all the particle systems
    CXMLTreeNode l_XML, l_Node;

    if ( l_XML.LoadAndFindNode( mConfigPath.c_str(), "particles_systems", l_Node ) )
    {
        mEmitterFactory->SetEmittersPath( l_Node.GetAttribute<std::string>( "emitters_path", "" ) );
        for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lCurrentSystem = l_Node( i );
            const std::string& lSystemCoreName = lCurrentSystem.GetAttribute<std::string>( "name", "" );

            CParticleSystemCore* lSystemCore = new CParticleSystemCore( lCurrentSystem, mEmitterFactory );

            ASSERT( lSystemCore, "Null PSystemCore" );

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
  BROFILER_CATEGORY( "CParticleSystemManager::Update()", Profiler::Color::Orchid )
    BOOST_FOREACH( CParticleSystemCore* lCore, m_ResourcesVector )
    {
        lCore->Update();
    }
}

void CParticleSystemManager::Render()
{
  BROFILER_CATEGORY( "CParticleSystemManager::Render()", Profiler::Color::Orchid )
    BOOST_FOREACH( CParticleSystemCore* lCore, m_ResourcesVector )
    {
        lCore->Render();
    }
}

void CParticleSystemManager::LoadInstances( const std::string& aFileName )
{
    CXMLTreeNode l_XML, l_Node;
    if ( l_XML.LoadAndFindNode( aFileName.c_str(), "particles_instances", l_Node ) )
    {
        for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lCurrentParticle = l_Node( i );
            const std::string& lNameParticle = lCurrentParticle.GetAttribute<std::string>( "name", "" );

            CParticleSystemCore * lCore = GetResource(lCurrentParticle.GetAttribute<std::string>( "core", "" ));
            ASSERT(lCore, "Triying to add a instance particle (%s) with out a core(%s)", lNameParticle.c_str(), lCurrentParticle.GetAttribute<std::string>( "core", "" ).c_str() );
            if(lCore)
                lCore->AddInstance( new CParticleInstance( lCurrentParticle ) );
        }
    }
}