#include "Particles\ParticleSystemCore.h"
#include "EngineConfig.h"
#include "Timer\Timer.h"
#include "Emitters\EmitterFactory.h"
#include "InstanceParticle.h"
#include <boost\foreach.hpp>
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CParticleSystemCore::CParticleSystemCore()
  : mDelayIn( 0.0 )
  , mCurrentTime( 0.0 )
{
}

CParticleSystemCore::CParticleSystemCore( const CXMLTreeNode& atts, CEmitterFactory* aEmitterFactory )
{
    for ( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lCurrentEmitter = atts( i );
        const std::string& lNameParticle  = lCurrentEmitter.GetAttribute<std::string>( "name", "" );
        AddEmitter( aEmitterFactory->CreateEmitter( lCurrentEmitter.GetName(), lCurrentEmitter ));
    }
}

CParticleSystemCore::~CParticleSystemCore()
{
    BOOST_FOREACH( CParticleEmitter* lParticleEmitter, mEmitters )
    {
        CHECKED_DELETE( lParticleEmitter );
    }

    BOOST_FOREACH( CParticleInstance* lParticleInstance, mInstances )
    {
        CHECKED_DELETE( lParticleInstance );
    }
}

void CParticleSystemCore::Update()
{
    float dt = deltaTimeMacro;
    mCurrentTime += dt;

    if( mCurrentTime > mDelayIn )
    {
      BOOST_FOREACH( CParticleEmitter* lParticleEmitter, mEmitters )
      {
        lParticleEmitter->Update(dt);
      }
    }
}

void CParticleSystemCore::Render()
{
    CGraphicsManager* lGM = GraphicsInstance;

    if( mCurrentTime > mDelayIn )
    {
      BOOST_FOREACH( CParticleInstance* lParticleInstance, mInstances )
      {
          if( lParticleInstance->IsVisible() )
          {
              lGM->SetTransform( lParticleInstance->GetTransform() );
              BOOST_FOREACH( CParticleEmitter* lParticleEmitter, mEmitters )
              {
                  lParticleEmitter->Render();
              }
          }
      }
    }

    // Set the indentity
    lGM->SetTransform( Math::Mat44f() );
}

void CParticleSystemCore::Refresh()
{
}

void CParticleSystemCore::SetFixedDirection( const Math::Vect3f& aDirection )
{
  BOOST_FOREACH( CParticleEmitter* lParticleEmitter, mEmitters )
  {
    lParticleEmitter->SetFixedDirection(aDirection);
  }
}

void CParticleSystemCore::AddInstance( CParticleInstance* aInstance )
{
    if( aInstance )
        mInstances.push_back(aInstance);
    else
        LOG_ERROR_APPLICATION("Adding a null instance to the core");
}

void CParticleSystemCore::AddEmitter( CParticleEmitter* aEmitter )
{
    if( aEmitter )
        mEmitters.push_back( aEmitter );
    else
        LOG_ERROR_APPLICATION("Adding a null emitter to the core");
}

void CParticleSystemCore::ResetEmitters()
{
  mCurrentTime = 0.0;
  BOOST_FOREACH( CParticleEmitter* lParticleEmitter, mEmitters )
  {
    lParticleEmitter->Reset();
  }
}