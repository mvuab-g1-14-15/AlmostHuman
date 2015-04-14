#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Particle.h"
#include "Math\Vector3.h"
#include "Utils\Defines.h"

#include <vector>

class CParticleEmitter
{
    protected:
        std::vector<CParticle> m_Particles;
        
        float m_MinLifetime;
        float m_MaxLifetime;

		float m_MinimumSize;
        float m_MaximumSize;

        float m_EmitterLifeTime;
		float m_TimeToEmit;
        float m_ActualTime;

		float m_Rand;
        float m_RandMin;
        float m_RandMax;

        Math::Vect3f m_Acceleration;
        Math::Vect3f m_Direction;
        Math::Vect3f m_Position;
        Math::Vect3f m_Velocity;
        
        bool m_Active;
        std::string m_TextureName;
              
    public:
        CParticleEmitter();
        virtual ~CParticleEmitter();
        
        void SetAcceleration( const Math::Vect3f& Acceleration );
        void SetVelocity( const Math::Vect3f& Velocity );
        
        void SetDirection( const Math::Vect3f& Direction );
        void SetPosition( const Math::Vect3f& Position );
        
        void SetTextureName(const std::string &Texture);
		void SetEmitterTime(float Time);

        float GetActive();
        void SetActive(bool Active);

        void SetLifeTime(float min, float max);
        void SetEmitterLifeTime(float LifeTime);

		void SetTimeToEmit(float Time);
		float GetTimeToEmit();

		void SetSize(float min, float max);
		float GetSize();

        virtual void Generate( unsigned int l_NumParticles ) = 0;
        virtual void Update( float dt ) = 0;
        virtual void Render() = 0;
};

#endif
