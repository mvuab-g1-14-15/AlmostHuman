#include "Particle.h"
#include <Windows.h>

#include "EngineManagers.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"
#include "Texture/TextureManager.h"
#include "Math/MathUtils.h"

CParticle::CParticle()
{
    ZeroMemory(this, sizeof(CParticle));
}

CParticle::~CParticle()
{

}

void CParticle::Update( float dt )
{
    mDirection = (mDirection * mSpeed + Math::Vect3f( 0, -1, 0) * mGravity * dt);
    mDirection.Normalize();
    mPosition += mDirection * mSpeed * dt + mOndSpeedDirection * Math::Utils::Sin( dt * mOndulationVel +
                 mInitalOndulation );
    mAngle += dt;
    mActualTime += dt;
}

void CParticle::Render()
{
    /*
        if ( mIsAlive )
        {
          mBillboard->Render();
        }
    */
}
