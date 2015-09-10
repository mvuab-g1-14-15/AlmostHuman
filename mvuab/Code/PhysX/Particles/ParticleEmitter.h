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

        const Math::Vect3f& GetMinPoint()
        {
            return mMinPnt;
        }

        const Math::Vect3f& GetMaxPoint()
        {
            return mMaxPnt;
        }
    protected:
        typedef CParticle*                TParticleContainer;
        typedef std::vector < CTexture*>  TTextureContainer;
        bool                        mIsLoop;
        bool                        mIsImmortal;
        bool                        mIsActive;
        uint32                      mAliveParticlesCount;
        float32                     mActualTime;
        float32                     mGravity;
        float32                     mOndSpeedDirectionMin;
        float32                     mOndSpeedDirectionMax;
        Math::Vect3f                mInitialDirectionMin;
        Math::Vect3f                mInitialDirectionMax;
        Math::CColor                mColorMin;
        Math::CColor                mColorMax;
        Math::Vect2f                mSpeed;
        Math::Vect2f                mRadialSpeed;
        Math::Vect2f                mOndSpeed;
        Math::Vect2u                mAliveParticles;
        Math::Vect2f                mEmissionTime;
        Math::Vect2f                mSize;
        Math::Vect2f                mTimeToLive;
        Math::Vect2f                mParticlesXEmission;
        CEffectTechnique*           mTechnique;
        TTextureContainer           mTextures;
        TParticleContainer          mParticles;
        TParticleContainer          mDeadParticles;
        Math::Vect3f                mMinPnt;
        Math::Vect3f                mMaxPnt;

        ps::TSpawnFunction          mSpawnFn;

        ps::TEmitterType            mType;

        CShape*                     mShape;

        CRenderableVertexs          *mRV;
        TPARTICLE_VERTEX_INSTANCE   *mParticlesStream;

    private:
        void EmitParticles();
        void KillParticles();
        void ActivateTextures();
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
