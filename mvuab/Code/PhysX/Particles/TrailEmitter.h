#ifndef _TRAIL_EMITTER_H
#define _TRAIL_EMITTER_H

#include "Particle.h"
#include "ParticleEmitter.h"

class CTrailEmitter : public CParticleEmitter
{
    private:
        float m_SizeX;
        float m_SizeY;
        
        float m_MinYaw;
        float m_MaxYaw;
        
        float m_MinPitch;
        float m_MaxPitch;

    public:
        CTrailEmitter			 ();
        virtual ~CTrailEmitter   ();

		void Update              (float dt);
		void Render              ();

        void Generate(unsigned int l_NumParticles);
        void NewParticle(CParticle* l_Particle);

        void SetSize(float sx, float sy);
        void SetPitch(float min, float max);
        void SetYaw(float min, float max);
		void SetRandom(float min, float max);
};

#endif
