#ifndef _PARTICLES_H
#define _PARTICLES_H
#pragma once

#include "Math\Vector3.h"
#include "Billboard\Billboard.h"

class CParticle
{
    public:
        CParticle();
		virtual ~CParticle();
        
        bool IsAlive();

		bool Init
		( 
			float aLifeTime, 
			const Math::Vect3f& aColor, 
			const Math::Vect3f& aPosition, 
			float aVelocity, 
			float aAceleration,
			const Math::Vect3f& aDirecction,
			float aSize,
			float aAngle,
			float aAlpha,
			const std::string & aTextureName,
			const std::string & aTechniqueName
		);

        void Update( float dt );
        void Render();		

	private:
		CBillboard *mBillboard;

		float mTime;
        float mLifeTime;
		bool  mIsAlive;

        Math::Vect3f mColor;
        Math::Vect3f mPosition;
		Math::Vect3f mDirection;
        float mVelocity;
        float mAcceleration;
};

//----------------------------------------------------------------------------------------
inline bool CParticle::IsAlive()
{
	return mIsAlive;
}

#endif
