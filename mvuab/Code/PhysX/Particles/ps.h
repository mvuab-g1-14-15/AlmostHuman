#pragma once
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Utils/BaseUtils.h"
#include "Texture/Texture.h"

#include <string>

class CParticleEmitter;
class CTexture;
class CEffectTechnique;

namespace ps {

	enum TEmitterType
	{
		eET_Circle = 0,
		eET_Sphere,
		eET_Line,
		eET_Point,
		eET_Box,
		eET_Radial
	};

	enum TSpeedType
	{
		eST_Linear = 0,
		eST_Radial,
		eST_Ondulative
	};

	struct EmitterSpeedPropery
	{
		Math::Vect2f mRange;				// A random value of this will be given to the particle
		Math::Vect3f mMin;					// The min direction rand range
		Math::Vect3f mMax;					// The max direction rand range
		float GetRange() { return baseUtils::RandRange( mRange.x, mRange.y); }
		Math::Vect3f GetRandomDirection() { return baseUtils::RandRange( mMin, mMax ).Normalize(); }
	};

	struct ParticleTexture
	{
		CTexture* mValue;					// Pointer to the texture object
		bool	  mFlipUVHorizontal;		// If the particle must randomize to flip horizontal
		bool	  mFlipUVVertical;			// If the particle must randomize to flip vertical
	};

	struct ParticleVisualProperty
	{
		CEffectTechnique* mTechnique;		// The techinque with the particle will be rendered
		Math::CColor      mMinColor;		// The min of thre range of the particles
		Math::CColor      mMaxColor;		// The max of the range of the particles
		float		      mAlpha;			// The alpha of the particles
		bool			  mKeepSize;		// Sets if the value of the size of the particle must not vary along it life
		Math::Vect2f	  mSizeRange;       // Random size when the particle is born
	};

	struct Emissions
	{
		float						mEmissionTime;
		bool						mModifyInitalAngle;
		Math::Vect2u				mParticlesPerEmission;
		Math::Vect3f			    mCubicSize;
		float					    mMinRadius;
		float						mMaxRadius;
		float						mAngleStep;

		uint32	GetParticlesToEmit() { return baseUtils::RandRange( mParticlesPerEmission.x, mParticlesPerEmission.y ); }
		float   Angle()			     { return (mModifyInitalAngle) ? baseUtils::RandRange(0.0f, 360.0f) : 0.0f; }
	};

	struct ParticlesProperties
	{
		ParticleVisualProperty mVisualProperty;
		ParticleTexture		   mTexture;
		uint32				   mMaxAliveParticles;
		Math::Vect2f	       mTimeToLive;

		void  ActivateTexture()			{ mTexture.mValue->Activate( 0 ); }
		bool  FlipUVHorizontal()		{ return (mTexture.mFlipUVHorizontal) ? baseUtils::RandomBool() : false; }
		bool  FlipUVVertical()			{ return (mTexture.mFlipUVVertical)   ? baseUtils::RandomBool() : false; }
		float Alpha()					{ return mVisualProperty.mAlpha; }
		float Size()					{ return baseUtils::RandRange(mVisualProperty.mSizeRange.x, mVisualProperty.mSizeRange.y); }
		float Size( float aPercentage )	{ return Math::Utils::Lerp<float32>(mVisualProperty.mSizeRange.x, mVisualProperty.mSizeRange.y, aPercentage); }
		float MaxLife()					{ return baseUtils::RandRange(mTimeToLive.x, mTimeToLive.y); }
		Math::CColor Color()			{ return baseUtils::RandRange(mVisualProperty.mMinColor,mVisualProperty.mMaxColor ); }
		bool KeepSize()					{ return mVisualProperty.mKeepSize; }
	};

typedef Math::Vect3f(*TSpawnFunction)(CParticleEmitter*);
}