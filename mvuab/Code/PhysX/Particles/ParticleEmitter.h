#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Particle.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Utils\Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Name.h"

class CParticle;

class CParticleEmitter :  public CName
{
    public:
        CParticleEmitter();
        virtual ~CParticleEmitter();

        virtual bool Init( const CXMLTreeNode& atts );
        void         Update( float dt );
        void         Render();
        bool IsActive();

        virtual Math::Vect3f GetSpawnPosition() = 0;

        const uint32 GetParticleCount() const;
        const CParticle* GetParticle( const uint32 aIdx) const;
        CParticle* GetParticle( const uint32 aIdx);

    protected:
        typedef std::vector<CParticle*> TParticleContainer;
        bool                        mIsLoop;
        bool                        mIsInmortal;
        bool                        mIsActive;
        uint32                      mMaxParticles;
        uint32                      mAliveParticles;
        float32                     mActualTime;
        Math::Vect2f                mTimeToEmit;
        Math::Vect2f                mAcceleration;
        Math::Vect2f                mVelocity;
        Math::Vect2f                mSize;
        Math::Vect2f                mTimeToLive;
        Math::Vect2f                mParticlesXEmission;
        Math::Vect3f                mDirection;
        Math::Vect3f                mPosition;
        std::string                 mTechniqueName;
        std::vector<std::string>    mTextures;
        TParticleContainer          mParticles;
        TParticleContainer          mDeadParticles;

    private:
        void EmitParticles();
        void KillParticles();
};

//-----------------------------------------------------------------------------------------
inline bool CParticleEmitter::IsActive()
{
    return mIsActive;
}

//-----------------------------------------------------------------------------------------
inline const uint32 CParticleEmitter::GetParticleCount() const
{
    return mParticles.size();
}

//-----------------------------------------------------------------------------------------
inline const CParticle* CParticleEmitter::GetParticle( const uint32 aIdx) const
{
    return mParticles[ aIdx ];
}

//-----------------------------------------------------------------------------------------
inline CParticle* CParticleEmitter::GetParticle( const uint32 aIdx)
{
    return mParticles[ aIdx ];
}


#endif
