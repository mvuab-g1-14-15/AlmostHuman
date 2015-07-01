#ifndef _PARTICLES_H
#define _PARTICLES_H
#pragma once

#include "Math\Vector3.h"
#include "Billboard\Billboard.h"
#include "Math/Color.h"

class CParticle
{
    public:
        CParticle();
        ~CParticle();

        void Update( float dt );
        void Render();

        bool IsAlive() const
        {
            return mIsAlive;
        }
        void SetIsAlive(bool val)
        {
            mIsAlive = val;
        }
        const Math::Vect3f& GetDirection() const
        {
            return mDirection;
        }
        void SetDirection(Math::Vect3f val)
        {
            mDirection = val;
        }
        float32 GetActualTime() const
        {
            return mActualTime;
        }
        void SetActualTime(float32 val)
        {
            mActualTime = val;
        }
        float32 GetGravity() const
        {
            return mGravity;
        }
        void SetGravity(float32 val)
        {
            mGravity = val;
        }
        const float32 GetSpeed() const
        {
            return mSpeed;
        }
        void SetSpeed(const float32 val)
        {
            mSpeed = val;
        }
        const float32 GetRadialSpeed() const
        {
            return mRadialSpeed;
        }
        void SetRadialSpeed(const float32 val)
        {
            mRadialSpeed = val;
        }
        const Math::Vect3f& GetOndSpeedDirection() const
        {
            return mOndSpeedDirection;
        }
        void SetOndSpeedDirection(Math::Vect3f val)
        {
            mOndSpeedDirection = val;
        }
        const float32 GetSize() const
        {
            return mSize;
        }
        void SetSize(const float val)
        {
            mSize = val;
        }
        const float32 GetTimeToLive() const
        {
            return mTimeToLive;
        }
        void SetTimeToLive(const float32 val)
        {
            mTimeToLive = val;
        }
        Math::Vect3f GetPosition() const
        {
            return mPosition;
        }
        void SetPosition(Math::Vect3f val)
        {
            mPosition = val;
        }
        Math::CColor GetColor() const
        {
            return mColor;
        }
        void SetColor(Math::CColor val)
        {
            mColor = val;
        }
        const float32 GetOndSpeed() const
        {
            return mOndSpeed;
        }
        void SetOndSpeed(const float32 val)
        {
            mOndSpeed = val;
        }
        float32 GetOndulationVel() const
        {
            return mOndulationVel;
        }
        void SetOndulationVel(float32 val)
        {
            mOndulationVel = val;
        }
        float32 GetInitalOndulation() const
        {
            return mInitalOndulation;
        }
        void SetInitalOndulation(float32 val)
        {
            mInitalOndulation = val;
        }
        float32 GetAngle() const
        {
            return mAngle;
        }
        void SetAngle(float32 val)
        {
            mAngle = val;
        }
    private:
        CBillboard *mBillboard;
        bool                        mIsAlive;
        float32                     mActualTime;
        float32                     mGravity;
        float32                     mSize;
        float32                     mOndSpeed;
        float32                     mSpeed;
        float32                     mRadialSpeed;
        float32                     mTimeToLive;
        float32                     mInitalOndulation;
        float32                     mOndulationVel;
        float32                     mAngle;
        Math::Vect3f                mDirection;
        Math::Vect3f                mOndSpeedDirection;
        Math::CColor                mColor;
        Math::Vect3f                mPosition;
};

#endif
