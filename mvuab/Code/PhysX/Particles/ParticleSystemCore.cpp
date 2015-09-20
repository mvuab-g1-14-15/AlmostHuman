#include "Particles\ParticleSystemCore.h"
#include "EngineConfig.h"
#include "Timer\Timer.h"
#include "Emitters\EmitterFactory.h"

CParticleSystemCore::CParticleSystemCore()
{
}

CParticleSystemCore::CParticleSystemCore( const CXMLTreeNode& atts, CEmitterFactory* aEmitterFactory )
{
    for ( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lCurrentEmitter = atts( i );
        const std::string& lNameParticle  = lCurrentEmitter.GetAttribute<std::string>( "name", "" );

        CParticleEmitter* lEmitter = aEmitterFactory->CreateEmitter( lCurrentEmitter.GetName(), lCurrentEmitter );
        ASSERT( lEmitter, "Null Emitter" );

        if ( !lEmitter || !AddResource( lEmitter->GetName(), lEmitter ) )
        {
            CHECKED_DELETE( lEmitter );
        }
    }
}

CParticleSystemCore::~CParticleSystemCore()
{
    Destroy();
}

void CParticleSystemCore::Update()
{
    for ( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i )
    {
        GetResourceById( i )->Update(deltaTimeMacro);
    }
}

void CParticleSystemCore::Render()
{
    for ( uint32 i = 0, lParticles = GetResourcesCount(); i < lParticles; ++i )
    {
        GetResourceById( i )->Render();
    }
}

void CParticleSystemCore::Refresh()
{
    Destroy();
}