#ifndef INC_CAMERAFREE_H_
#define INC_CAMERAFREE_H_

#include "Camera.h"
#include <d3dx9math.h>

class CCameraFree : public CCamera
{
public:
    CCameraFree();
    ~CCameraFree();
    void    Update( );

    void    Move (    int strafe, int forward, bool flag_speed, const float32 &dt);

    void    AddYaw( float32 radians );
    void    AddPitch( float32 radians );
    void    AddHeight( float32 amount );

    Math::Vect3f GetDirection( void ) const { return GetLookAt() - m_Position;}
    Math::Vect3f GetLookAt( void ) const;
    Math::Vect3f GetVecUp( void ) const;

    GET_SET( float32, Height )
    GET_SET( float32, YawSpeed )
    GET_SET( float32, PitchSpeed )
    GET_SET( float32, Speed )
    GET_SET( float32, ForwardSpeed )
    GET_SET( float32, StrafeSpeed )

private: // Members
    float32 m_Height;
    float32 m_YawSpeed;
    float32 m_PitchSpeed;
    float32 m_Speed;
    float32 m_ForwardSpeed;
    float32 m_StrafeSpeed;
};

#endif //INC_CAMERAFREE_H_