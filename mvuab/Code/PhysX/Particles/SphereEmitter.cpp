#include "SphereEmitter.h"
#include "Math\Matrix44.h"
#include "Utils/BaseUtils.h"

#include <omp.h>

CSphereEmitter::CSphereEmitter()
	: CParticleEmitter()
{
}

CSphereEmitter::~CSphereEmitter()
{
}

Math::Vect3f CSphereEmitter::GetSpawnPosition()
{
	return Math::Vect3f();
}

bool CSphereEmitter::Init( const CXMLTreeNode& atts )
{
	return CParticleEmitter::Init( atts );
}

/*
void CSphereEmitter::NewParticle(CParticle*)
{
	/*
    float l_Pitch = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxPitch));
    float l_Yaw = Math::Utils::Deg2Rad(baseUtils::RandRange(m_MinPitch, m_MaxYaw));
    
    float l_Radius = baseUtils::RandRange(m_MinimumRadius, m_MaximumRadius);
    float l_LifeTime = baseUtils::RandRange(m_MinLifetime, m_MaxLifetime);
    
    float x =  Math::Utils::Cos(l_Yaw) *  Math::Utils::Cos(l_Yaw + Math::pi32 * 0.5f);
    float y =  Math::Utils::Sin(l_Pitch);
    float z =  Math::Utils::Sin(l_Yaw) + Math::Utils::Sin(l_Yaw + Math::pi32 * 0.5f);
    
    Math::Vect3f l_Vector = Math::Vect3f(x, y, z).Normalize();
    l_Particle->SetAcceleration(m_Acceleration);
    l_Particle->SetVelocity(l_Vector);
    
    l_Particle->SetTextureName(m_TextureName);
    l_Particle->SetSize(m_SizeX, m_SizeY);
    
    l_Particle->SetPosition(m_Position + l_Radius);
    l_Particle->SetLifeTime(l_LifeTime);
    l_Particle->SetIsAlive(true);
}
*/