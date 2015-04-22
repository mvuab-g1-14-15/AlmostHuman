//----------------------------------------------------------------------------------
// CJump class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de gestionar la física del salto del Player.
//
//    a(t) = cte = g
//    v(t) = g*t + v0
//    y(t) = g*t^2 + v0*t + y0
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_JUMP_H_
#define INC_JUMP_H_

class CJump
{
    public:
        CJump();
        ~CJump(){ }

        void  StartJump   ( float v0, float m_height );
        void  StopJump    ();

        float GetHeight   ( float elapsedTime );
        float GetVelocity () { return m_fV0; }

    private:
        bool  m_bJump;

        float m_fV0;
        float m_height;
        float m_fJumpTime;
        float m_fDefaultGravity;
};

#endif // INC_JUMP_H_