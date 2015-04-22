#ifndef INC_CAMERACINEMATICAL_H_
#define INC_CAMERACINEMATICAL_H_

#include "Camera.h"
#include <d3dx9math.h>

class CCameraKeyController;

class CCameraCinematical : public CCamera
{
public:
    CCameraCinematical(CCameraKeyController *KeyController);
    ~CCameraCinematical();
    void    Update( );

    Math::Vect3f GetDirection( void ) const { return GetLookAt() - m_Position;}

    void SetLookAt( const Math::Vect3f & LookAt  ){m_LookAt = LookAt;}
    Math::Vect3f GetLookAt( void ) const;

    void SetVecUp( const Math::Vect3f & Up){m_Up = Up;}
    Math::Vect3f GetVecUp( void ) const;

private: // Members
    CCameraKeyController *m_pKeyController;
    Math::Vect3f                    m_LookAt;
    Math::Vect3f                    m_Up;
};

#endif //INC_CAMERAFPSHOOTER_H_