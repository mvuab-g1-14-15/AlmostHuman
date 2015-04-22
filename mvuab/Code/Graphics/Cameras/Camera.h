#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "Frustum.h"
#include "Utils\Name.h"
#include "Object3D.h"

#include <d3dx9.h>

class CCamera : public CName, public CObject3D
{
public:

    enum Type{ Free = 0, ThirdPerson, FirstPerson, Spherical, Cinematical };

    CCamera();
    virtual ~CCamera();

    void            RenderCamera( LPDIRECT3DDEVICE9 device);
    virtual void    Update( );
    virtual Math::Vect3f GetDirection( void ) const;
    virtual Math::Vect3f GetLookAt( void ) const;
    virtual Math::Vect3f GetVecUp( void ) const;

    D3DXMATRIX GetMatrixView( void );
    D3DXMATRIX  GetMatrixProj( void );

    GET_SET( Type, CameraType )
    GET_SET( float32, ZNear )
    GET_SET( float32, ZFar )
    GET_SET( float32, AspectRatio )
    GET_SET( bool, Enable )

    //void    AddPos( const D3DXVECTOR3 &position ) { m_Pos += position; }
    void    AddYaw( float32 aRadian );
    void    AddPitch( float32 aRadian );
    void    AddZf( float32 aAmount );
    void    AddFov( float32 aAmont );
    void    AddViewD( float32 aAmount );

    // Frustum methods
    GET_SET( CFrustum, Frustum );
    void UpdateFrustum(D3DXMATRIX ViewProjectionMatrix);

protected:

    float32            m_view_d;
    float32            m_FovInRadians;
    float32            m_AspectRatio;
    float32            m_ZNear;
    float32            m_ZFar;

    Type               m_CameraType;

    bool               m_Enable;

    CFrustum           m_Frustum;

    D3DXMATRIXA16      m_view;
    D3DXMATRIXA16      m_proj;
};

#endif // INC_CAMERA_H_