#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Math\Vector3.h"
#include "Particle.h"

#include <vector>

class CParticleEmitter
{
    protected:
        std::vector<CParticle> m_Particles;
        
        float m_MinSpeed;
        float m_MaxSpeed;

        float m_MinLifetime;
        float m_MaxLifetime;

        Math::Vect3f m_Acceleration;
        Math::Vect3f m_Direction;
        Math::Vect3f m_Position;
        Math::Vect3f m_Velocity;

    public:
        CParticleEmitter    ();
        ~CParticleEmitter   ();

        void SetAcceleration    (const Math::Vect3f &Acceleration);
        void SetVelocity        (const Math::Vect3f &Velocity);

        void SetDirection       (const Math::Vect3f &Direction);
        void SetPosition        (const Math::Vect3f &Position);
        
        void SetTimeToLive      (float min, float max);

        void Update             (float dt);
        void Render             ();

        virtual void Generate  (unsigned int l_NumParticles) = 0;
};

#endif
