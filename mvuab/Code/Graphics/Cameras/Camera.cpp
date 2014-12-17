#include "Camera.h"
#include "GraphicsManager.h"

D3DXMATRIX CCamera::GetMatrixView(void) 
{

  D3DXMatrixLookAtLH( &m_view, 
                      &GetEye(),
                      &GetLookAt(), 
                      &GetVecUp());
  return m_view;
}

D3DXMATRIX CCamera::GetMatrixProj(void)
{

   D3DXMatrixPerspectiveFovLH( &m_proj, 
                               m_fov_radians, 
                               m_aspect_ratio, 
                               m_zn, 
                               m_zf );
  return m_proj;
}

void CCamera::RenderCamera(LPDIRECT3DDEVICE9 device) 
{
    D3DXMATRIX matrix;
    D3DXMATRIX translation;

    D3DXMatrixTranslation( &translation, m_pos.x ,m_pos.y ,m_pos.z );
    device->SetTransform( D3DTS_WORLD, &translation );
        
    CGraphicsManager::GetSingletonPtr()->DrawCube(5.0f, colRED);

    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    device->SetTransform( D3DTS_WORLD, &matrix );

    //---------PINTAMOS EL FRUSTUM-------------//
       D3DXMATRIX translation2;
    D3DXVECTOR3 eye = GetEye();
    D3DXMatrixTranslation( &translation, eye.x ,eye.y ,eye.z );
                        
    D3DXMATRIX rotation;
    D3DXMATRIX rotation2;
    
    
    D3DXMatrixRotationY ( &rotation,  -m_yaw);
    D3DXMatrixRotationZ ( &rotation2, +m_pitch);

    matrix = rotation2 * rotation * translation;

    // Cambiar el sistema de coordenadas
    device->SetTransform( D3DTS_WORLD, &matrix );

    struct CAMERA_VERTEX 
    {
        D3DXVECTOR3 loc;
        DWORD color;
        static unsigned int getFlags() 
        { 
            return (D3DFVF_XYZ|D3DFVF_DIFFUSE); 
        }
    };

    CAMERA_VERTEX pts[9];
    
    float32 d = m_view_d;

    float32 h_near = m_zn * tan ( m_fov_radians * 0.5f );
    float32 k_near = h_near * m_aspect_ratio;

    float32 h_far = d * tan ( m_fov_radians * 0.5f );
    float32 k_far = h_far * m_aspect_ratio;

    pts[ 0 ].loc = D3DXVECTOR3( 0, 0,0 );
    pts[ 0 ].color = 0xffffffff;

    pts[ 1 ].loc = D3DXVECTOR3( d, h_far, k_far );
    pts[ 1 ].color = 0xffffffff;
    pts[ 2 ].loc = D3DXVECTOR3( d, h_far, -k_far );
    pts[ 2 ].color = 0xffffffff;
    pts[ 3 ].loc = D3DXVECTOR3( d,-h_far, -k_far );
    pts[ 3 ].color = 0xffffffff;
    pts[ 4 ].loc = D3DXVECTOR3( d, -h_far, k_far );
    pts[ 4 ].color = 0xffffffff;

    pts[ 5 ].loc = D3DXVECTOR3( m_zn, h_near, k_near );
    pts[ 5 ].color = 0xffffffff;
    pts[ 6 ].loc = D3DXVECTOR3( m_zn, h_near, -k_near );
    pts[ 6 ].color = 0xffffffff;
    pts[ 7 ].loc = D3DXVECTOR3( m_zn,-h_near, -k_near );
    pts[ 7 ].color = 0xffffffff;
    pts[ 8 ].loc = D3DXVECTOR3( m_zn, -h_near, k_near );
    pts[ 8 ].color = 0xffffffff;

    // Decimos el tipo de vertice que vamos a proporcionar...
    device->SetFVF( CAMERA_VERTEX::getFlags() );

    // Desactivar la textura
    device->SetTexture (0, NULL);

    device->DrawPrimitiveUP( D3DPT_POINTLIST, 9, pts, sizeof( CAMERA_VERTEX ) );
    static short int indexes[] =  {
        0,1, 0,2, 0,3, 0,4,
        1,2, 2,3, 3,4, 4,1,
        5,6, 6,7, 7,8, 8,5
    };
    device->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 9, 12, indexes, D3DFMT_INDEX16, pts, sizeof( CAMERA_VERTEX ) );
    
    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    device->SetTransform( D3DTS_WORLD, &matrix );

    
    
    //---------PINTAMOS LA BOX Y LOS EJES------------------//
    D3DXMatrixTranslation( &translation2, -1.0f, 0.0f, 0.0f );
    matrix = translation2 * rotation2 * rotation * translation; 
    device->SetTransform( D3DTS_WORLD, &matrix );

    struct AXIS_VERTEX
    {
        float32 x,y,z;
        DWORD color;
        static unsigned int getFlags()
        {
            return(D3DFVF_XYZ|D3DFVF_DIFFUSE);
        }
    };    
    AXIS_VERTEX v[6] =
    {
        //EJE X
        {0, 0, 0, 0xffff0000},
        {3, 0, 0, 0xffff0000},
        //EJE Y
        {0, 0, 0, 0xff00ff00},
        {0, 3, 0, 0xff00ff00},
        //EJE Z
        {0, 0, 0, 0xff0000ff},
        {0, 0, 3, 0xff0000ff},
    };

    device->SetTexture(0,NULL);
    device->SetFVF(AXIS_VERTEX::getFlags());
    device->DrawPrimitiveUP( D3DPT_LINELIST, 3, v,sizeof(AXIS_VERTEX));
    
    CGraphicsManager::GetSingletonPtr()->DrawCube(5.0f, colRED);

    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    device->SetTransform( D3DTS_WORLD, &matrix );
}

D3DXVECTOR3 CCamera::Yaw2vector( const float32 &yaw_angle ) const
{
    return D3DXVECTOR3( cos(yaw_angle), 0.0f, sin(yaw_angle) );
}

D3DXVECTOR3 CCamera::YawPitch2vector( const float32 &yaw_angle, const float32 &pitch_angle ) const
{
    return D3DXVECTOR3( sin(yaw_angle), sin(pitch_angle), cos(yaw_angle) );
}

void CCamera::Update( float32 deltaTime )
{
}