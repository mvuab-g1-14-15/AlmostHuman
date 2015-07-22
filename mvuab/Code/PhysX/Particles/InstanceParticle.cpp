#include "Particles\InstanceParticle.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleSystemCore.h"
#include "GraphicsManager.h"

CInstanceParticle::CInstanceParticle( const CXMLTreeNode& atts )
  : CObject3D( atts )
  , mCore( ParticleSystemInstance->GetResource( atts.GetAttribute<std::string>( "core", "" ) ) )
{
  ASSERT( mCore, "Null core" );
}

CInstanceParticle::~CInstanceParticle()
{

}

void CInstanceParticle::Render()
{
  CGraphicsManager* lGM = GraphicsInstance;
  lGM->SetTransform( GetTransform() );
  //lGM->EnableAlphaBlend();
  //lGM->EnableZTest();
  lGM->SetSrcBlend( "One" );
  lGM->SetDstBlend( "One" );
  lGM->SetBlendOP( "Add" );
  mCore->Render();
  lGM->SetTransform( Math::Mat44f() );
}