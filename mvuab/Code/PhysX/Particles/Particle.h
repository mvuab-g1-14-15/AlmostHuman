#ifndef _PARTICLES_H
#define _PARTICLES_H
#pragma once

#include "Math\Vector3.h"

class CParticle
{
    private:
        float m_Size;
        float m_TimeToLive;

        bool  m_IsAlive;

        Math::Vect3f m_Color;
        Math::Vect3f m_Position;

        Math::Vect3f m_Velocity;
        Math::Vect3f m_Aceleration;

    public:
        CParticle ();
        CParticle (float sz, float timer, const Math::Vect3f &Color, const Math::Vect3f &Position, const Math::Vect3f &Velocity, const Math::Vect3f &Aceleration);

        void SetColor                       (const Math::Vect3f &Color);
        const Math::Vect3f &GetColor        ();

        void SetPosition                    (const Math::Vect3f &Position);
        const Math::Vect3f &GetPostion      ();

        void SetVelocity                    (const Math::Vect3f &Velocity);
        const Math::Vect3f &GetVelocity     ();

        void SetAceleration                 (const Math::Vect3f &Aceleration);
        const Math::Vect3f &GetAceleration  ();

        void SetTimeToLive                  (float Time);
        float GetTimeToLive                 ();

        void SetIsAlive                     (bool isAlive);
        bool GetIsAlive                     ();

        void Update                         (float dt);
};

#endif
