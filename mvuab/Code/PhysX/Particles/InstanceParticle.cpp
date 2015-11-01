#include "Particles\InstanceParticle.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleSystemCore.h"
#include "GraphicsManager.h"

CParticleInstance::CParticleInstance( const CXMLTreeNode& atts )
    : CObject3D( atts )
    , CName( atts.GetAttribute<std::string>("name", "null_name") )
    , mRoomName( atts.GetAttribute<std::string>("room", "null_room") )
{
  mCore       = PSManager->GetPSCore( atts.GetAttribute<std::string>("core", "null_core") );
  mIsVisible  = atts.GetAttribute<bool>( "active", false );
  mIsOk = mCore != NULL;

  ASSERT( mIsOk, "The instance is not ok %s, %s", GetName().c_str(),atts.GetAttribute<std::string>("core", "null_core").c_str() );
}

CParticleInstance::CParticleInstance( const std::string& aName, const std::string& aCoreName, const std::string& aRoomName )
  : CName( aName )
  , mRoomName( aRoomName )
  , mCore( PSManager->GetPSCore( aCoreName ) )
{
  mIsOk = mCore != NULL;
  ASSERT( mIsOk, "The instance is not ok %s, %s", aName.c_str(), aCoreName.c_str() );
}

CParticleInstance::~CParticleInstance()
{
  CHECKED_DELETE( mCore );
}

void CParticleInstance::Update( float dt )
{
  if( mIsVisible )
    mCore->Update( dt );
}

void CParticleInstance::Render()
{
  if(mIsVisible)
  {
    CGraphicsManager* lGM = GraphicsInstance;
    lGM->SetTransform( GetTransform() );
    mCore->Render();
    lGM->SetTransform( Math::Mat44f() );
  }
}

void CParticleInstance::Reset()
{
  mCore->ResetEmitters();
}

void CParticleInstance::SetDirection( const Math::Vect3f& aDirection )
{
  mCore->SetFixedDirection( aDirection );
}