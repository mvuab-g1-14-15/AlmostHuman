#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "Camera.h"

class CCameraFPShooter : public CCamera 
{

public:

    CCameraFPShooter();
    CCameraFPShooter( const D3DXVECTOR3 &InitialPosition, const D3DXVECTOR3 &TargetPoint );
    ~CCameraFPShooter();
    void    Update( float32 deltaTime );
    
    void    Move (    int strafe, int forward, bool flag_speed, const float32 &dt);
        
    void    AddYaw( const float32 &radians );
    void    AddPitch( const float32 &radians );
    void    AddY( const float32 &amount ) { m_Pos.y = m_posY + amount; }
    
    Math::Vect3f GetDirection( void ) const { return GetLookAt() - m_Pos;}
    Math::Vect3f GetLookAt( void ) const;
    Math::Vect3f GetVecUp( void ) const;
    
private:

    float32 m_posY;            //Altura de la camara
    float32 m_speed_yaw;        //Velocidad a la que gira la camara respecto el angulo Yaw
    float32 m_speed_pitch;    //Velocidad a la que gita la camara respecto el angulo Pitch
    float32 m_speed;            //Velocidad a la que se desplaza lateralmente y adelante/atras la camara
    float32 m_speed_forward;    //Velocidad a la que se despleza hacia adelante/atras la camara
    float32 m_speed_strafe;    //Velocidad a la que se desplaza lateralmente la camara
};

#endif //INC_CAMERAFPSHOOTER_H_