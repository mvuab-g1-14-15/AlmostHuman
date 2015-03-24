#ifndef _SPHERE_EMITTER_H
#define _SPHERE_EMITTER_H

#include "Particle.h"
#include "ParticleEmitter.h"

class CSphereEmitter : public CParticleEmitter
{
    private:
        float m_MinimumRadius;
        float m_MaximumRadius;

        float m_MinYaw;
        float m_MaxYaw;

        float m_MinPitch;
        float m_MaxPitch;

    public:
        CSphereEmitter    ();
        ~CSphereEmitter   ();

        void Generate (unsigned int l_NumParticles);
        void NewParticleSphere(CParticle* l_Particle);
        void SetRadius(float min, float max);
        void SetYaw(float min, float max);
        void SetPitch(float min, float max);

};

#endif
