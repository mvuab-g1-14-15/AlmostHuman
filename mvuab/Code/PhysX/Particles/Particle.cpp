#include "Particle.h"
#include <Windows.h>

#include "EngineManagers.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"
#include "Texture/TextureManager.h"

CParticle::CParticle()
	: mTime(0.0f)
	, mLifeTime( 0.0f )
	, mIsAlive( false )
	, mColor( Math::Vect3f( 0.0f, 0.0f, 0.0f ) )
	, mPosition( Math::Vect3f( 0.0f, 0.0f, 0.0f ) )
	, mVelocity( 1.0f )
	, mAcceleration( 0.0f )
	, mBillboard( 0 )
	, mDirection( Math::Vect3f( 0.0f, 0.0f, 0.0f ) )
{
}

CParticle::~CParticle()
{
	CHECKED_DELETE( mBillboard );
}

bool CParticle::Init
( 
	float aLifeTime, 
	const Math::Vect3f& aColor, 
	const Math::Vect3f& aPosition, 
	float aVelocity, 
	float aAcceleration,
	const Math::Vect3f& aDirecction,
	const Math::Vect2f& aSize,
    const std::string & aTextureName,
    const std::string & aTechniqueName
)
{
	mLifeTime = aLifeTime;
	mIsAlive = true;
	mColor = aColor;
	mPosition = aPosition;
	mVelocity = aVelocity;
	mAcceleration = aAcceleration;
	mDirection = aDirecction.GetNormalized();

	mBillboard = new CBillboard();

	bool lOk = mBillboard->Init( "ParticleBillboard", aPosition, aSize, aTextureName, aTechniqueName );

	//ASSERT( lOk, "Could not init the particle")

	return lOk;
}

void CParticle::Update( float dt )
{
	ASSERT( mBillboard, "Null billboard for particle" );

	//if( mIsAlive )
	{
		mTime += dt;

		//if( mTime < mLifeTime )
		//{
			float lVi = mVelocity;
			mVelocity += mAcceleration * dt;
			mPosition = mPosition + mDirection * ( mVelocity + lVi * 0.5f * dt );
			mBillboard->SetPosition( mPosition );
			mBillboard->Update();
		//}
		//else
		//{
			//mIsAlive = false;
		//}
	}
}

void CParticle::Render()
{
	mBillboard->Render();
}
