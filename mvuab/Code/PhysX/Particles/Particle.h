#ifndef _PARTICLES_H
#define _PARTICLES_H
#pragma once

#include "Math\Vector3.h"
#include "Billboard\Billboard.h"

class CParticle
{
    private:
        float m_LifeTime;
        float m_SphereRadius;

        bool  m_IsAlive;
        CBillboard m_Billboard;

        Math::Vect3f m_Color;
        Math::Vect3f m_Position;

        Math::Vect3f m_Velocity;
        Math::Vect3f m_Acceleration;

    public:
        CParticle();
        CParticle( float sz, float timer, const Math::Vect3f& Color, const Math::Vect3f& Position, const Math::Vect3f& Velocity, const Math::Vect3f& Aceleration );

        void SetTextureName( std::string TextureName );
        void SetSize( float sx, float sy );

        void SetColor( const Math::Vect3f& Color );
        const Math::Vect3f& GetColor();

        void SetPosition( const Math::Vect3f& Position );
        const Math::Vect3f& GetPosition();

        void SetVelocity( const Math::Vect3f& Velocity );
        const Math::Vect3f& GetVelocity();

        void SetAcceleration( const Math::Vect3f& Acceleration );
        const Math::Vect3f& GetAcceleration();

        void SetLifeTime( float Time );
        float GetLifeTime();

        void SetIsAlive( bool isAlive );
        bool GetIsAlive();

        void Update( float dt );
        void Render();
};

#endif
