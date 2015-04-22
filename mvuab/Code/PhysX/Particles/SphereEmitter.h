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

        float m_Rand;
        float m_RandMin;
        float m_RandMax;

    public:
        CSphereEmitter();
        virtual ~CSphereEmitter();

        void Update(float dt);
        void Render();

        void Generate(unsigned int l_NumParticles, bool l_Generate = true);
        void NewParticle(CParticle *l_Particle);

        void SetRandom(float min, float max);
        void SetRadius(float min, float max);
        void SetPitch(float min, float max);
        void SetYaw(float min, float max);
};

#endif
