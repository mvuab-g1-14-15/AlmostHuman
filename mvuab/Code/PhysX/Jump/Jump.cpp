//#include "Utils\Base.h"
#include "Jump\Jump.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CJump::CJump() : m_bJump( false )
  , m_fV0( 0.0f )
  , m_height( 0.0f )
  , m_fJumpTime( 0.0f )
  , m_fDefaultGravity( -9.81f )
{
}

void CJump::StartJump( float v0, float height )
{
    if ( m_bJump ) return;
    m_bJump  = true;

    m_fJumpTime = 0.0f;
    m_height = height;
    m_fV0  = v0;
}

void CJump::StopJump()
{
    if ( !m_bJump ) return;
    m_bJump = false;
}

float CJump::GetHeight( float elapsedTime )
{
    /*
    a(t) = cte = g
    v(t) = g*t + v0
    y(t) = g*t^2 + v0*t + y0
    */

    if ( !m_bJump ) return 0.0f;
    m_fJumpTime += elapsedTime;

    float l_Vel = m_fV0 + m_fDefaultGravity * m_fJumpTime;
    float l_Height = m_height + m_fV0 * m_fJumpTime + 0.5f * m_fDefaultGravity * m_fJumpTime * m_fJumpTime;

    return l_Height - m_height;
}