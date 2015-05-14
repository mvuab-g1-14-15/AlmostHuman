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
        void		 Update( float dt );
        void	     Render();
		
		virtual Math::Vect3f GetSpawnPosition() = 0;

		bool IsActive();

    protected:
        bool                    mIsLoop;
		bool					mIsInmortal;
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
};

//-----------------------------------------------------------------------------------------
inline bool CParticleEmitter::IsActive()
{
	return mIsActive;
}

#endif
