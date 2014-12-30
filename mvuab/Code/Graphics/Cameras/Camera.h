#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "Utils\Name.h"

#include <d3dx9.h>

class CCameraInfo;

class CCamera : public CName
{

public:

    enum Type{ FREE = 0, ThirdPerson, FirstPerson, Spherical, Cinematical };

    CCamera();
    virtual ~CCamera();

    void            RenderCamera( LPDIRECT3DDEVICE9 device);
    virtual void    Update( ) = 0;

    virtual Math::Vect3f GetDirection( void ) const = 0;
    virtual Math::Vect3f GetLookAt( void ) const = 0;
    virtual Math::Vect3f GetVecUp( void ) const = 0;

    D3DXMATRIX GetMatrixView( void );
    D3DXMATRIX  GetMatrixProj( void );

    GET_SET( float32, Yaw )
    GET_SET( float32, Pitch )
    GET_SET( float32, ZNear )
    GET_SET( float32, ZFar )
    GET_SET( float32, aspect_ratio )
    GET_SET_REF( Math::Vect3f, Pos )

    void SetInfo( CCameraInfo* CameraInfo );
    
    //void    AddPos( const D3DXVECTOR3 &position ) { m_Pos += position; }
    void    AddYaw( float32 radian ){ m_Yaw += D3DXToRadian(radian); }
    void    AddPitch( float32 radian ){ m_Pitch += D3DXToRadian(radian); }
    void    AddZf( float32 amount ){ m_ZNear += amount; }
    void    AddFov( float32 delta_fov ){ m_fov_radians += delta_fov; }
    void    AddViewD( float32 amount ){ if( m_view_d + amount > 1) m_view_d += amount; }

protected:

    Math::Vect3f       m_Pos;
    float32            m_Yaw;
    float32            m_Pitch;

    float32            m_view_d;
    float32            m_fov_radians;
    float32            m_aspect_ratio;
    float32            m_ZNear;
    float32            m_ZFar;

    D3DXMATRIXA16      m_view;
    D3DXMATRIXA16      m_proj;
};

#endif // INC_CAMERA_H_