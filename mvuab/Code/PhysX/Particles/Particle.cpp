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

    /*
        ASSERT( mBillboard, "Null billboard for particle" );

        if ( mIsAlive )
        {
          mTime += dt;

          if ( mTime < mLifeTime )
          {
              /*
                  Math::CLerpAnimator1D lAlphaInterpolator;
                  lAlphaInterpolator.SetValues(A.GetFarPlane(), B.GetFarPlane(), 1.0f, Math::FUNC_CONSTANT);
                  float32 l_FarPlane;
                  l_Interpolator1D.Update(Percentage, l_FarPlane);


              float lVi = mVelocity;
              mVelocity += mAcceleration * dt;
              mPosition = mPosition + mDirection * ( mVelocity + lVi * 0.5f * dt );
              mBillboard->SetPosition( mPosition );
              mBillboard->Update();
          }
          else
          {
              mIsAlive = false;
          }
        }
    */
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
