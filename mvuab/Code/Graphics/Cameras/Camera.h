#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CCamera
{

public:

    CCamera(){};
    virtual ~CCamera(){};

    void            RenderCamera( LPDIRECT3DDEVICE9 device);
    virtual void    Update( float32 deltaTime ) = 0;

    virtual Math::Vect3f GetDirection( void ) const = 0;
    virtual Math::Vect3f GetLookAt( void ) const = 0;
    virtual Math::Vect3f GetVecUp( void ) const = 0;

    D3DXMATRIX GetMatrixView( void );
    D3DXMATRIX  GetMatrixProj( void );

    GET_SET( float32, yaw )
    GET_SET( float32, pitch )
    GET_SET( float32, zn )
    GET_SET( float32, zf )
    GET_SET_REF( Math::Vect3f, Pos )
    
    //void    AddPos( const D3DXVECTOR3 &position ) { m_Pos += position; }
    void    AddYaw( const float32 &radian ){ m_yaw += D3DXToRadian(radian); }
    void    AddPitch( const float32 &radian ){ m_pitch += D3DXToRadian(radian); }
    void    AddZf( const float32 &amount ){ m_zf += amount; }
    void    AddFov( const float32 &delta_fov ){ m_fov_radians += delta_fov; }
    void    AddViewD( const float32 &amount ){ if( m_view_d + amount > 1) m_view_d += amount; }

protected:
    
    Math::Vect3f       m_Pos;
    float32            m_yaw;
    float32            m_pitch;

    float32            m_view_d;
    float32            m_fov_radians;
    float32            m_aspect_ratio;
    float32            m_zn;
    float32            m_zf;

    D3DXMATRIXA16      m_view;
    D3DXMATRIXA16      m_proj;
};

#endif // INC_CAMERA_H_