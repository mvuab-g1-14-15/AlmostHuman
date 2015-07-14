#include "Particle.h"
#include <Windows.h>

#include "EngineManagers.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"
#include "Texture/TextureManager.h"
#include "Math/MathUtils.h"

CParticle::CParticle()
    : mIsAlive( false )
    , mActualTime(0.0f)
    , mGravity(1.0f)
    , mSize( 1.0f )
    , mOndSpeed( 1.0f)
    , mSpeed(1.0f )
    , mRadialSpeed( 1.0f)
    , mTimeToLive( 0.0f )
    , mInitalOndulation( 1.0f )
    , mOndulationVel( 1.0f )
    , mAngle( 0.0f )
    , mDirection( Math::Vect3f() )
    , mOndSpeedDirection( Math::Vect3f() )
    , mColor( Math::CColor() )
    , mPosition( Math::Vect3f() )
{
}

CParticle::~CParticle()
{

}

void CParticle::Update( float dt )
{
    mDirection = (mDirection * mSpeed + Math::Vect3f( 0, -1, 0) * mGravity * dt);
    mDirection.Normalize();
    mPosition += mDirection * mSpeed * dt;
    mAngle += dt * mRadialSpeed;
    mActualTime += dt;
}