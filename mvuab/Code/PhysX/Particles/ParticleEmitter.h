#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Particle.h"
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "Utils\Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Name.h"
#include "RenderableVertex\InstancedVertexTypes.h"
#include "ps.h"

class CParticle;
class CTexture;
class CRenderableVertexs;
class CEffectTechnique;
class CShape;

class CParticleEmitter :  public CName
{
    public:
        CParticleEmitter( ps::TEmitterType aType, ps::TSpawnFunction aFunction );
        virtual ~CParticleEmitter();

        virtual bool Init( const CXMLTreeNode& atts );
        void         Update( float dt );
        void         Render();
        bool IsActive();

        const uint32 GetParticleCount() const;
        const CParticle* GetParticle( const uint32 aIdx) const;
        CParticle* GetParticle( const uint32 aIdx);

		ps::Emissions GetEmissions() const { return mEmissions;}

    private:
		ps::TEmitterType            mType;

        typedef CParticle*                TParticleContainer;
		TParticleContainer				  mParticles;
        TParticleContainer                mDeadParticles;

		ps::EmitterSpeedPropery		      mLinearSpeed;
		ps::EmitterSpeedPropery			  mRadialSpeed;
		ps::EmitterSpeedPropery			  mOndulativeSpeed;

		uint32							  mAliveParticlesCount;		// The number of alive particles
		ps::ParticlesProperties           mParticleProperies;
		ps::Emissions					  mEmissions;
		ps::TSpawnFunction                mSpawnFn;

        bool							  mIsLoop;					// If the emitter after his dead has to born again, as the Phoenix ;D
        bool							  mIsActive;				// If the emitter is active

		float32						      mTimeSinceLastEmission;   // The time since the emitter has emitted some particles
        float32                           mMaxLife;					// The maxium life of the emitter
        float32                           mActualTime;			    // The current time of the emitter
        float32                           mGravity;
        
        CRenderableVertexs                *mRV;
        TPARTICLE_VERTEX_INSTANCE         *mParticlesStream;

    private:
        void EmitParticles();
        void KillParticles();
        void ActivateTextures();
		void LoadFromNode( const CXMLTreeNode& atts );
};

//-----------------------------------------------------------------------------------------
inline bool CParticleEmitter::IsActive()
{
    return mIsActive;
}

//-----------------------------------------------------------------------------------------
inline const uint32 CParticleEmitter::GetParticleCount() const
{
    return 0;//return mParticles.size();
}

//-----------------------------------------------------------------------------------------
inline const CParticle* CParticleEmitter::GetParticle( const uint32 aIdx) const
{
    return 0;
}

//-----------------------------------------------------------------------------------------
inline CParticle* CParticleEmitter::GetParticle( const uint32 aIdx)
{
    return 0;
}


#endif
