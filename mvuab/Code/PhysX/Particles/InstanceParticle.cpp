#include "Particles\InstanceParticle.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleSystemCore.h"

CInstanceParticle::CInstanceParticle()
{
}

CInstanceParticle::CInstanceParticle( const CXMLTreeNode& atts ) :
  m_ParticleSystem( ParticleSystemInstance->GetResource( atts.GetAttribute<std::string>( "core", "" ) ) ),
  m_Position( atts.GetAttribute<Math::Vect3f>( "position", Math::Vect3f( 0, 0, 0 ) ) )
{	
}

CInstanceParticle::~CInstanceParticle()
{

}

void CInstanceParticle::Update()
{
  m_ParticleSystem->Update(m_Position);
}

void CInstanceParticle::Render()
{
  m_ParticleSystem->Render();
}