#ifndef _CUBE_EMITTER_H
#define _CUBE_EMITTER_H

#include "Particle.h"
#include "ParticleEmitter.h"

class CCubeEmitter : public CParticleEmitter
{
    private:
        float m_MinWidth;
        float m_MaxWidth;

        float m_MinHeight;
        float m_MaxHeight;

        float m_MinDepth;
        float m_MaxDepth;


    public:
        CCubeEmitter			();
        virtual ~CCubeEmitter   ();

		void Update             (float dt);
		void Render             ();

        void Generate (unsigned int l_NumParticles);
        void NewParticle(CParticle* l_Particle);

        void SetDepth(float min, float max);
        void SetWidth(float min, float max);
        void SetHeight(float min, float max);
		void SetRandom(float min, float max);
};

#endif
