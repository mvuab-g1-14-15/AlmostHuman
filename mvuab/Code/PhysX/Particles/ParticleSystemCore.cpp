#include "Particles\ParticleSystemCore.h"
#include "EngineConfig.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"
#include "Timer\Timer.h"


CParticleSystemCore::CParticleSystemCore()
{
}

CParticleSystemCore::CParticleSystemCore( const CXMLTreeNode& atts )
{
  for ( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
  {
    const CXMLTreeNode& lCurrentEmitter = atts( i );
    const std::string& lNameParticle  = lCurrentEmitter.GetAttribute<std::string>( "name", "" );
    const std::string& lEmitterType   = lCurrentEmitter.GetName();
    CParticleEmitter* lEmitter = 0;

    if ( lEmitterType == "cube_emitter" )
      lEmitter =  new CCubeEmitter();
    else if ( lEmitterType == "sphere_emitter" )
      lEmitter =  new CSphereEmitter();

    ASSERT( lEmitter, "Null Emitter" );

    if ( !lEmitter->Init( lCurrentEmitter ) || ( !AddResource( lEmitter->GetName(), lEmitter ) ) )
    {
      LOG_ERROR_APPLICATION( "Error initing emitter %s", lEmitter->GetName() );
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
  //LO ACTUALIZA EL INSTANCE? TODO ALEX
  for ( uint32 i = 0, lEmitters = GetResourcesCount(); i < lEmitters; ++i )
  {
    CParticleEmitter* lEmitter = GetResourceById( i );

    if ( lEmitter->IsActive() )
      lEmitter->Update( deltaTimeMacro );
  }
}

void CParticleSystemCore::Render()
{

}

void CParticleSystemCore::Refresh()
{
  Destroy();
  Init();
}