#include "ParticleManager.h"
#include "Timer\Timer.h"
#include "EngineConfig.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"


CParticleManager::CParticleManager() : CManager()
{
}

CParticleManager::CParticleManager(CXMLTreeNode& atts) : CManager(atts)
{
}

CParticleManager::~CParticleManager()
{
    Destroy();
}

void CParticleManager::Init()
{
    CXMLTreeNode l_XML;
    if ( !l_XML.LoadFile( mConfigPath.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "CParticleManager::Init Can't load XML file" );
        return;
    }

    CXMLTreeNode l_Node = l_XML["emitters"];

    if ( !l_Node.Exists() )
    {
        LOG_ERROR_APPLICATION( "CParticleManager::Init Can't find node \"emitters\"" );
        return;
    }

    for(int i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i)
    {
        if(!l_Node.IsComment())
        {
            const std::string& lEmitterType = l_Node( i ).GetPszProperty( "type", "cube" );

            ASSERT( lEmitterType != "", "Null emitter type");

            CParticleEmitter *lEmitter = 0;

            if( lEmitterType == "sphere")
            {
                lEmitter =  new CSphereEmitter();
            }
            else if(lEmitterType == "cube")
            {
                lEmitter =  new CCubeEmitter();
            }

            ASSERT(lEmitter, "Null Emitter");

            if( !lEmitter->Init( l_Node( i ) ) || ( !AddResource( lEmitter->GetName(), lEmitter ) ) )
            {
                LOG_ERROR_APPLICATION( "Error initing emitter %s", lEmitter->GetName() );
                CHECKED_DELETE( lEmitter );
            }
        }
    }
}

void CParticleManager::Update()
{
    for( uint32 i = 0, lEmitters = GetResourcesCount(); i < lEmitters; ++i)
    {
        CParticleEmitter* lEmitter = GetResourceById( i );
        if(lEmitter->IsActive())
        {
            lEmitter->Update( deltaTimeMacro );
        }
    }
}

void CParticleManager::Render()
{
    for( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i)
    {
        CParticleEmitter* lEmitter = GetResourceById( i );
        if(lEmitter->IsActive())
        {
            lEmitter->Render();
        }
    }
}

void CParticleManager::Refresh()
{
    Destroy();
    Init();
}