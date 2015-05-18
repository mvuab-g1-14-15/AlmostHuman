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
        
        bool GetIsAlive();

		bool Init
		( 
			float aLifeTime, 
			const Math::Vect3f& aColor, 
			const Math::Vect3f& aPosition, 
			const Math::Vect3f& aVelocity, 
			const Math::Vect3f& aAceleration,
			const Math::Vect2f& aSize,
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
        Math::Vect3f mVelocity;
        Math::Vect3f mAcceleration;
};

#endif
