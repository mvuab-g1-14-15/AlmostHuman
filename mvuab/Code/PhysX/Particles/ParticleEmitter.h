#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Math\Vector3.h"
#include "Particle.h"


#include <vector>

class CParticleEmitter
{
    private:
        std::vector<CParticle> m_Particles;
        float m_TimeToLive;

        Math::Vect3f m_Acceleration;
        Math::Vect3f m_Direction;
        Math::Vect3f m_Position;
        Math::Vect3f m_Velocity;


    public:
        CParticleEmitter    ();
        ~CParticleEmitter   ();

        void SetAcceleration    (const Math::Vect3f &Acceleration);
        void SetDirection       (const Math::Vect3f &Direction);
        void SetPosition        (const Math::Vect3f &Position);
        void SetVelocity        (const Math::Vect3f &Velocity);
        void SetTimeToLive      (float timeToLive);


        void Update             (float dt);
        void Render             ();

        void Generate           (unsigned int numParticles);
        void NewParticle        ();
};

#endif