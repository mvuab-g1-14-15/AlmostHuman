#include "Camera.h"
#include "CameraInfo.h"
#include "GraphicsManager.h"
#include "Utils/BaseUtils.h"

#include "EngineManagers.h"

CCamera::CCamera()
    : CObject3D()
    , m_view_d( 2.0f )
    , m_FovInRadians( Math::Utils::Deg2Rad( 60.0f ) )
    , m_ZNear( 0.1f )
    , m_ZFar( 1000.0f )
    , m_Enable( true )
{
    uint32 w, h;
    GraphicsInstance->GetWidthAndHeight( w, h );
    m_AspectRatio = ( float ) w / ( float ) h;
}

CCamera::~CCamera()
{
}

void CCamera::Update( )
{
}

Math::Vect3f CCamera::GetDirection( void ) const
{
    return Math::Vect3f();
}

Math::Vect3f CCamera::GetLookAt( void ) const
{
    return Math::Vect3f();
}

Math::Vect3f CCamera::GetVecUp( void ) const
{
    return Math::Vect3f();
}

D3DXMATRIX CCamera::GetMatrixView( void )
{
    Math::Vect3f& l_LookAt = GetLookAt();
    Math::Vect3f& l_UpVec = GetVecUp();
    D3DXVECTOR3 d3dxPos( m_Position.x, m_Position.y, m_Position.z );
    D3DXVECTOR3 d3dxTarget( l_LookAt.x, l_LookAt.y, l_LookAt.z );
    D3DXVECTOR3 d3dxUp( l_UpVec.x, l_UpVec.y, l_UpVec.z );
    D3DXMatrixLookAtLH( &m_view, &d3dxPos, &d3dxTarget, &d3dxUp );
    return m_view;
}

D3DXMATRIX CCamera::GetMatrixProj( void )
{
    D3DXMatrixPerspectiveFovLH( &m_proj, m_FovInRadians, m_AspectRatio, m_ZNear, m_ZFar );
    return m_proj;
}

void CCamera::RenderCamera( LPDIRECT3DDEVICE9 device )
{
    #ifdef _DEBUG
    D3DXMATRIX matrix;
    D3DXMATRIX translation;

    if ( m_CameraType != FirstPerson )
    { GraphicsInstance->DrawSphere( GetLookAt(), 0.1f, Math::colRED, 20 ); }

    //Reset the translation
    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    GraphicsInstance->SetTransform( matrix );
    //---------PINTAMOS EL FRUSTUM-------------//
    D3DXMATRIX translation2;
    D3DXVECTOR3 eye( m_Position.x, m_Position.y, m_Position.z );
    D3DXMatrixTranslation( &translation, eye.x , eye.y , eye.z );
    D3DXMATRIX rotation;
    D3DXMATRIX rotation2;
    D3DXMatrixRotationY( &rotation,  - m_fYaw );
    D3DXMatrixRotationZ( &rotation2, + m_fPitch );
    matrix = rotation2 * rotation * translation;
    // Cambiar el sistema de coordenadas
    GraphicsInstance->SetTransform( matrix );
    struct CAMERA_VERTEX
    {
        D3DXVECTOR3 loc;
        DWORD color;
        static unsigned int getFlags()
        {
            return ( D3DFVF_XYZ | D3DFVF_DIFFUSE );
        }
    };
    CAMERA_VERTEX pts[9];
    float32 d = m_view_d;
    float32 h_near = m_ZNear * tan( m_FovInRadians * 0.5f );
    float32 k_near = h_near * m_AspectRatio;
    float32 h_far = d * tan( m_FovInRadians * 0.5f );
    float32 k_far = h_far * m_AspectRatio;
    pts[ 0 ].loc = D3DXVECTOR3( 0, 0, 0 );
    pts[ 0 ].color = 0xffffffff;
    pts[ 1 ].loc = D3DXVECTOR3( d, h_far, k_far );
    pts[ 1 ].color = 0xffffffff;
    pts[ 2 ].loc = D3DXVECTOR3( d, h_far, -k_far );
    pts[ 2 ].color = 0xffffffff;
    pts[ 3 ].loc = D3DXVECTOR3( d, -h_far, -k_far );
    pts[ 3 ].color = 0xffffffff;
    pts[ 4 ].loc = D3DXVECTOR3( d, -h_far, k_far );
    pts[ 4 ].color = 0xffffffff;
    pts[ 5 ].loc = D3DXVECTOR3( m_ZNear, h_near, k_near );
    pts[ 5 ].color = 0xffffffff;
    pts[ 6 ].loc = D3DXVECTOR3( m_ZNear, h_near, -k_near );
    pts[ 6 ].color = 0xffffffff;
    pts[ 7 ].loc = D3DXVECTOR3( m_ZNear, -h_near, -k_near );
    pts[ 7 ].color = 0xffffffff;
    pts[ 8 ].loc = D3DXVECTOR3( m_ZNear, -h_near, k_near );
    pts[ 8 ].color = 0xffffffff;
    // Decimos el tipo de vertice que vamos a proporcionar...
    device->SetFVF( CAMERA_VERTEX::getFlags() );
    // Desactivar la textura
    device->SetTexture( 0, NULL );
    device->DrawPrimitiveUP( D3DPT_POINTLIST, 9, pts, sizeof( CAMERA_VERTEX ) );
    static short int indexes[] =
    {
        0, 1, 0, 2, 0, 3, 0, 4,
        1, 2, 2, 3, 3, 4, 4, 1,
        5, 6, 6, 7, 7, 8, 8, 5
    };
    device->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 9, 12, indexes, D3DFMT_INDEX16, pts, sizeof( CAMERA_VERTEX ) );
    GraphicsInstance->DrawCube( matrix, 0.1f, Math::colRED );
    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    GraphicsInstance->SetTransform( matrix );
    //---------PINTAMOS LA BOX Y LOS EJES------------------//
    D3DXMatrixTranslation( &translation2, -1.0f, 0.0f, 0.0f );
    matrix = translation2 * rotation2 * rotation * translation;
    GraphicsInstance->SetTransform( matrix );
    struct AXIS_VERTEX
    {
        float32 x, y, z;
        DWORD color;
        static unsigned int getFlags()
        {
            return ( D3DFVF_XYZ | D3DFVF_DIFFUSE );
        }
    };
    AXIS_VERTEX v[6] =
    {
        //EJE X
        {0, 0, 0, 0xffffff00},
        {3, 0, 0, 0xffffff00},
        //EJE Y
        {0, 0, 0, 0xff00ff00},
        {0, 3, 0, 0xff00ff00},
        //EJE Z
        {0, 0, 0, 0xff00ffff},
        {0, 0, 3, 0xff00ffff},
    };
    device->SetTexture( 0, NULL );
    device->SetFVF( AXIS_VERTEX::getFlags() );
    device->DrawPrimitiveUP( D3DPT_LINELIST, 3, v, sizeof( AXIS_VERTEX ) );
    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    GraphicsInstance->SetTransform( matrix );
    #endif
}

void CCamera::UpdateFrustum( D3DXMATRIX ViewProjectionMatrix )
{
    m_Frustum.Update( ViewProjectionMatrix );
}

void CCamera::AddYaw( float32 aRadian )
{
    m_fYaw += D3DXToRadian( aRadian );
}
void CCamera::AddPitch( float32 aRadian )
{
    m_fPitch += D3DXToRadian( aRadian );
}
void CCamera::AddZf( float32 aAmont )
{
    m_ZNear += aAmont;
}
void CCamera::AddFov( float32 aDeltaFov )
{
    m_FovInRadians += aDeltaFov;
}

void CCamera::AddViewD( float32 aAmount )
{
    if ( m_view_d + aAmount > 1 )
    { m_view_d += aAmount; }
}