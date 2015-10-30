#include "ParticleSystemManager.h"
#include "EngineConfig.h"
#include "Emitters/EmitterFactory.h"
#include "Timer/Timer.h"

#include <boost/foreach.hpp>


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
    CHECKED_DELETE( mEmitterFactory );
    mCores.Destroy();
    mInstances.Destroy();
}

void CParticleSystemManager::Refresh()
{
  LoadXML();
  LoadInstances( "Data/sala3/particles.xml", "sala3" ); // The refresh only is available in developer mode
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

            if ( lSystemCoreName == "" || !mCores.AddResource( lSystemCoreName, lSystemCore ) )
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
  float dt = deltaTimeMacro;
  BOOST_FOREACH( CParticleInstance* lInstance, mInstances.GetResourcesVector() )
  {
    lInstance->Update(dt);
  }
}

void CParticleSystemManager::Render()
{
  BROFILER_CATEGORY( "CParticleSystemManager::Render()", Profiler::Color::Orchid )
  BOOST_FOREACH( CParticleInstance* lInstance, mInstances.GetResourcesVector() )
  {
    lInstance->Render();
  }
}

void CParticleSystemManager::LoadInstances( const std::string& aFileName, const std::string& aRoomName )
{
    CXMLTreeNode l_XML, l_Node;
    if ( l_XML.LoadAndFindNode( aFileName.c_str(), "particles_instances", l_Node ) )
    {
        for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
        {
            CParticleInstance* lInstance = new CParticleInstance( l_Node( i ) );
            lInstance->SetRoomName( aRoomName );
            if( !lInstance->IsOk() || !mInstances.AddResource( lInstance->GetName() + aRoomName, lInstance ) )
            {
              LOG_ERROR_APPLICATION("Error adding the instance %s to the particle system manager of room %s", lInstance->GetName().c_str(), aRoomName.c_str() );
              CHECKED_DELETE( lInstance );
            }
        }
    }
}

CParticleSystemCore* CParticleSystemManager::GetPSCore( const std::string & aCoreName )
{
  return mCores.GetResource( aCoreName )->Clone();
}