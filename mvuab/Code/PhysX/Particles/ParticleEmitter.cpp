#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"

#include "Core.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

#include <omp.h>

CParticleEmitter::CParticleEmitter()
{
    m_Particles.resize(0);

    m_MinSpeed = 0;
    m_MaxSpeed = 0;
    
    m_MinLifetime = 0;
    m_MaxLifetime = 0;

    m_Position = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Direction = Math::Vect3f(0.0f, 0.0f, 0.0f);
    m_Velocity = Math::Vect3f(0.0f, 0.0f, 0.0f);
}

CParticleEmitter::~CParticleEmitter()
{
    m_Particles.clear();
}

void CParticleEmitter::Update(float dt)
{
    CParticle *p = &m_Particles[0];
    omp_set_num_threads(2);

    #pragma omp parallel for
    for(int i = 0; i < m_Particles.size(); i++)
    {
        (p + i)->Update(dt);
    }
}

void CParticleEmitter::Render()
{
    CCameraManager *l_CM = CCore::GetSingletonPtr()->GetCameraManager();
    for(std::vector<CParticle>::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
    {
        if(l_CM->GetCurrentCamera()->GetFrustum().BoxVisibleByVertexs(&it->GetPosition()))
            it->Render();
    }
}

void CParticleEmitter::SetAcceleration(const Math::Vect3f &Acceleration)
{
    m_Acceleration = Acceleration;
}

void CParticleEmitter::SetDirection(const Math::Vect3f &Direction)
{
    m_Direction = Direction;
}

void CParticleEmitter::SetPosition(const Math::Vect3f &Position)
{
    m_Position = Position;
}
void CParticleEmitter::SetVelocity(const Math::Vect3f &Velocity)
{
    m_Velocity = Velocity;
}

void CParticleEmitter::SetTimeToLive(float min, float max)
{
    m_MinLifetime = min;
    m_MaxLifetime =  max;
}
