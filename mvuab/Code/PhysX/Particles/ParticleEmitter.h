#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "Utils\Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Name.h"
#include "RenderableVertex\InstancedVertexTypes.h"
#include "Interpolators\ParticleInterpolator.h"
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
  const CParticle* GetParticle( const uint32 aIdx ) const;
  CParticle* GetParticle( const uint32 aIdx );

public:
  Math::Vect3f                mCubicSize;
  float32                      mAngleStep;
  float32                      mCurrentAngle;
  float32                      mRadiusMin;
  float32                      mRadiusMax;

private:
  float*                        mParticlesLifeTime;
  float*                        mParticlesTTL;
  float*                        mParticlesAngles;
  float*                        mParticlesSpeed;
  float*                        mParticlesRadialSpeed;
  bool*                         mParticlesIsAlive;
  bool*                         mParticlesFlipUVHorizontal;
  bool*                         mParticlesFlipUVVertical;
  Math::Vect3f*                 mParticlesPositions;
  Math::Vect3f*                 mParticlesDirections;


  struct Motion
  {
    Math::Vect2f                mLinearSpeed;
    Math::Vect2f                mRadialSpeed;
    Math::Vect3f                mInitialDirectionMin;
    Math::Vect3f                mInitialDirectionMax;
    float                       mGravity;
  };

  struct ParticleTexture
  {
    CTexture* mValue;          // Pointer to the texture object
    bool   mFlipUVHorizontal;    // If the particle must randomize to flip horizontal
    bool   mFlipUVVertical;      // If the particle must randomize to flip vertical
  };

  ps::TEmitterType            mType;
  uint32                      mMaxAliveParticles;
  Math::Vect2f                mTTLParticles;
  float                       mTTLEmitter;
  CEffectTechnique*           mTechnique;
  ParticleTexture             mTexture;
  Motion                      mMotion;
  CParticleInterpolator       mInterpolator;
  bool                        mRandomInitialAngle;
  Math::Vect2u                mParticlesPerEmission;


  uint32                            mAliveParticlesCount;   // The number of alive particles
  ps::TSpawnFunction                mSpawnFn;

  bool                              mIsLoop;          // If the emitter after his dead has to born again, as the Phoenix ;D
  bool                              mIsActive;        // If the emitter is active

  float32                           mEmissionTime;
  float32                           mTimeSinceLastEmission;    // The time since the emitter has emitted some particles
  float32                           mMaxLife;                  // The maxium life of the emitter
  float32                           mActualTime;               // The current time of the emitter

  CRenderableVertexs*                mRV;
  TPARTICLE_VERTEX_INSTANCE*         mParticlesStream;

private:
  void EmitParticles();
  void KillParticles();
  void ActivateTextures();
  void LoadFromNode( const CXMLTreeNode& atts );
};

//-----------------------------------------------------------------------------------------
inline const uint32 CParticleEmitter::GetParticleCount() const
{
  return 0;//return mParticles.size();
}

//-----------------------------------------------------------------------------------------
inline const CParticle* CParticleEmitter::GetParticle( const uint32 aIdx ) const
{
  return 0;
}

//-----------------------------------------------------------------------------------------
inline CParticle* CParticleEmitter::GetParticle( const uint32 aIdx )
{
  return 0;
}


#endif
