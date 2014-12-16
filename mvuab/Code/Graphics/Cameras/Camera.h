#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <d3dx9.h>
#include "Utils\Types.h"

class CCamera
{

public:

    CCamera(){};
    virtual ~CCamera(){};

    // Render (para debug) de la posicion y la orientacion de la camara
    void    RenderCamera    ( LPDIRECT3DDEVICE9 device);
    virtual void    Update( float32 deltaTime );

    //--- GET FUNCTIONS ---
    virtual D3DXVECTOR3 GetDirection( void ) const = 0;
    virtual D3DXVECTOR3 GetLookAt( void ) const = 0;
    virtual D3DXVECTOR3 GetEye( void ) const = 0;
    virtual D3DXVECTOR3 GetVecUp( void ) const = 0;
    virtual D3DXVECTOR3 GetPos( void ) const = 0;
    float32 GetYaw( void ) const { return m_yaw; }
    float32 GetPitch( void ) const { return m_pitch; }

    D3DXMATRIX GetMatrixView( void );
    D3DXMATRIX  GetMatrixProj( void );
    
    void    SetYaw( const float32 &new_yaw ){ m_yaw  = new_yaw; }
    void    SetPitch( const float32 &new_pitch ){ m_pitch  = new_pitch; }
    void    SetPos( const D3DXVECTOR3 &new_pos ){ m_pos = new_pos; }
    void    SetZn( const float32 &amount ){ m_zn = amount; }
    
    void    AddPos( const D3DXVECTOR3 &position ) { m_pos += position; }
    void    AddYaw( const float32 &radian ){ m_yaw += D3DXToRadian(radian); }
    void    AddPitch( const float32 &radian ){ m_pitch += D3DXToRadian(radian); }
    void    AddZf( const float32 &amount ){ m_zf += amount; }
    void    AddFov( const float32 &delta_fov ){ m_fov_radians += delta_fov; }
    void    AddViewD( const float32 &amount ){ if( m_view_d + amount > 1) m_view_d += amount; }

    //--- AUXILIARY FUNCTIONS ---
    D3DXVECTOR3            Yaw2vector        ( const float32 &yaw_angle ) const;
    D3DXVECTOR3            YawPitch2vector ( const float32 &yaw_angle, const float32 &pitch_angle ) const;        


protected:
    
    D3DXVECTOR3        m_pos;               // posicion de la camara
    float32            m_yaw;               // angulo yaw de la camara
    float32            m_pitch;             // angulo pitch de la camara

    float32            m_view_d;            // variable de debug utilizada para pintar el objeto de la camara.    
    float32            m_fov_radians;
    float32            m_aspect_ratio;
    float32            m_zn;                // valor del z near (a partir de que vemos)
    float32            m_zf;                // valor del z far (hasta donde podemos ver)
    
    D3DXMATRIXA16    m_view;                // matriz vista
    D3DXMATRIXA16    m_proj;                // matriz proyeccion
};

#endif // INC_CAMERA_H_