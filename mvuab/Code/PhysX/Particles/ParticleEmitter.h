#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Particle.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Utils\Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Name.h"

class CParticle;

class CParticleEmitter : public CTemplatedVectorMapManager< CParticle >, public CName
{
    public:
        CParticleEmitter();
        virtual ~CParticleEmitter();

        virtual bool Init( const CXMLTreeNode& atts );

        void SetAcceleration( const Math::Vect3f& Acceleration );
        void SetVelocity( const Math::Vect3f& Velocity );

        void SetDirection( const Math::Vect3f& Direction );
        void SetPosition( const Math::Vect3f& Position );

        void SetTextureName(const std::string &Texture);
        void SetEmitterTime(float Time);

        float GetActive();
        void SetActive(bool Active);

        void SetLifeTime(float sx, float sy);
        void SetEmitterLifeTime(float LifeTime);

        void SetTimeToEmit(float Time);
        float GetTimeToEmit();

        void SetSize(float min, float max);
        float GetSize();

        void SetOrientate(bool l_Orientate);

        virtual void Generate( unsigned int l_NumParticles, bool l_Generate = true ) = 0;
        virtual void Update( float dt ) = 0;
        virtual void Render() = 0;

    protected:
        bool                    mIsLoop;
        bool                    mIsActive;
        uint32                  mMaxParticles;
        uint32                  mParticlesCount;
        float32                 mTimeToLive;
        float32                 mTimeToEmit;
        float32                 mActualTime;
        Math::Vect2f            mSize;
        Math::Vect3f            mAcceleration;
        Math::Vect3f            mDirection;
        Math::Vect3f            mPosition;
        Math::Vect3f            mVelocity;
        CEffectTechnique*       mTechnique;
        std::vector<CTexture*>  mTextures;

        std::vector<CParticle> m_Particles;

        float m_MinLifetime;
        float m_MaxLifetime;

        float m_EmitterLifeTime;
        float m_TimeToEmit;
        float m_ActualTime;

        float m_SizeX;
        float m_SizeY;

        float m_Rand;
        float m_RandMin;
        float m_RandMax;

        Math::Vect3f m_Acceleration;
        Math::Vect3f m_Direction;
        Math::Vect3f m_Position;
        Math::Vect3f m_Velocity;

        bool m_Active;
        bool m_Orientate;
        std::string m_TextureName;
};

#endif
