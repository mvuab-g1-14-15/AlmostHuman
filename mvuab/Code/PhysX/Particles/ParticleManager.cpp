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
            const CXMLTreeNode& lCurrentEmitter = l_Node( i );
            const std::string& lEmitterType = lCurrentEmitter.GetAttribute<std::string>( "type", "cube" );

            CParticleEmitter* lEmitter = 0;

            if ( lEmitterType == "cube" )
                lEmitter =  new CCubeEmitter();
            else if ( lEmitterType == "sphere" )
                lEmitter =  new CSphereEmitter();

            ASSERT( lEmitter, "Null Emitter" );

            if ( !lEmitter->Init( lCurrentEmitter ) || ( !AddResource( lEmitter->GetName(), lEmitter ) ) )
            {
                LOG_ERROR_APPLICATION( "Error initing emitter %s", lEmitter->GetName() );
                CHECKED_DELETE( lEmitter );
            }
        }
    }
}

void CParticleManager::Update()
{
    for ( uint32 i = 0, lEmitters = GetResourcesCount(); i < lEmitters; ++i )
    {
        CParticleEmitter* lEmitter = GetResourceById( i );

        if ( lEmitter->IsActive() )
            lEmitter->Update( deltaTimeMacro );
    }
}

void CParticleManager::Render()
{
    for ( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i )
    {
        CParticleEmitter* lEmitter = GetResourceById( i );

        if ( lEmitter->IsActive() )
            lEmitter->Render();
    }
}

void CParticleManager::Refresh()
{
    Destroy();
    Init();
}