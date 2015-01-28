#include "GraphicsManager.h"
#include "Utils\Defines.h"
#include "Math\MathTypes.h"
#include "Logger\Logger.h"
#include "Cameras\CameraManager.h"
#include "Cameras\Camera.h"
#include "Math\Vector3.h"
#include <malloc.h>
#include <vector>
#include "Exceptions\Exception.h"
#include "InputManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Core.h"


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
typedef struct CUSTOMVERTEX
{
  float x, y, z;
  DWORD color;

  static unsigned int getFlags()
  {
    return ( D3DFVF_CUSTOMVERTEX );
  }
} CUSTOMVERTEX;

typedef struct CUSTOMVERTEX2
{
  D3DXVECTOR3 pos;
  DWORD color;
  static unsigned int getFlags()
  {
    return D3DFVF_CUSTOMVERTEX;
  }
} CUSTOMVERTEX2;

struct SCREEN_COLOR_VERTEX
{
  float x, y, z, w;
  DWORD color;
  float u, v;

  static unsigned int getFlags()
  {
    return ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
  }
};

struct VERTEX2
{
  D3DXVECTOR3    pos;
  DWORD       d;

  static unsigned int getFlags()
  {
    return ( D3DFVF_XYZ | D3DFVF_DIFFUSE );
  }
};


CGraphicsManager::CGraphicsManager() :
  m_uWidth( CCore::GetSingletonPtr()->GetScreenHeight() ),
  m_uHeight( CCore::GetSingletonPtr()->GetScreenWidth() ),
  m_BackbufferColor_debug( Math::colBLUE ),
  m_BackbufferColor_release( Math::colBLACK ),
  m_bPaintSolid( true ),
  m_TeapotMesh( 0 )
{
  m_CylinderMesh = 0;
  m_SphereMesh = 0;
  m_CylinderMesh = 0;

  
}

CGraphicsManager::~CGraphicsManager()
{
  Release();
}

void CGraphicsManager::Update()
{
}

void CGraphicsManager::Render()
{
}

void CGraphicsManager::Release()
{
  CHECKED_RELEASE( m_SphereMesh );
  CHECKED_RELEASE( m_BoxMesh );
  CHECKED_RELEASE( m_CylinderMesh );
  CHECKED_RELEASE( m_TeapotMesh );
}

void CGraphicsManager::BeginRender()
{
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
  uint32 red        = ( uint32 )( m_BackbufferColor_debug.GetRed() * 255 );
  uint32 green    = ( uint32 )( m_BackbufferColor_debug.GetGreen() * 255 );
  uint32 blue        = ( uint32 )( m_BackbufferColor_debug.GetBlue() * 255 );
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
  uint32 red        = ( uint32 )( m_BackbufferColor_release.GetRed() * 256 );
  uint32 green    = ( uint32 )( m_BackbufferColor_release.GetGreen() * 256 );
  uint32 blue        = ( uint32 )( m_BackbufferColor_release.GetBlue() * 256 );
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#endif

  // Begin the scene
  HRESULT hr = m_pD3DDevice->BeginScene();
  assert( SUCCEEDED( hr ) );
  m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
  m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

  if ( m_bPaintSolid )
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
  else
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
}

void CGraphicsManager::Clear()
{
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
  uint32 red        = ( uint32 )( m_BackbufferColor_debug.GetRed() * 255 );
  uint32 green    = ( uint32 )( m_BackbufferColor_debug.GetGreen() * 255 );
  uint32 blue        = ( uint32 )( m_BackbufferColor_debug.GetBlue() * 255 );
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
  uint32 red        = ( uint32 )( m_BackbufferColor_release.GetRed() * 256 );
  uint32 green    = ( uint32 )( m_BackbufferColor_release.GetGreen() * 256 );
  uint32 blue        = ( uint32 )( m_BackbufferColor_release.GetBlue() * 256 );
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#endif
}

void CGraphicsManager::EndRender()
{
  m_pD3DDevice->EndScene();
  // Present the backbuffer contents to the display
  m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CGraphicsManager::SetupMatrices()
{
  Math::Vect3f l_CameraPosition;
  D3DXMATRIX l_matView;
  D3DXMATRIX l_matProject;

  // Obtain the current camera from the camera manager
  CCamera* l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();

  //Set default view and projection matrix
  if ( !l_CurrentCamera )
  {
    // Harcoded position
    l_CameraPosition = Math::Vect3f( 0.0f, 5.0f, -5.0f );

    //Setup Matrix view
    D3DXVECTOR3 l_Eye( 0.0f, 5.0f, -5.0f ), l_LookAt( 0.0f, 0.0f, 0.0f ), l_VUP( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &l_matView, &l_Eye, &l_LookAt, &l_VUP );

    //Setup Matrix projection
    D3DXMatrixPerspectiveFovLH( &l_matProject, 45.0f * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );
  }
  else
  {
    l_CameraPosition = l_CurrentCamera->GetPos();
    l_matView        = l_CurrentCamera->GetMatrixView();
    l_matProject     = l_CurrentCamera->GetMatrixProj();
    l_CurrentCamera->UpdateFrustum( l_matView * l_matProject );
  }

  m_pD3DDevice->SetTransform( D3DTS_VIEW, &l_matView );
  m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &l_matProject );

  // Set the new parameters to the effect manager
  CEffectManager::GetSingletonPtr()->ActivateCamera( Math::Mat44f( l_matView ), Math::Mat44f( l_matProject ),
      l_CameraPosition );
}

bool CGraphicsManager::Init( HWND hWnd, bool fullscreenMode, uint32 widthScreen, uint32 heightScreen )
{
  m_WindowId = hWnd;
  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, "RenderManager:: Inicializando la libreria Direct3D" );
  // Create the D3D object.
  m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
  assert( m_pD3D );
  m_bIsOk = bool( m_pD3D != NULL );

  if ( m_bIsOk )
  {
    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

    if ( fullscreenMode )
    {
      d3dpp.Windowed          = FALSE;
      d3dpp.BackBufferWidth   = widthScreen;
      d3dpp.BackBufferHeight  = heightScreen;
      d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
    }
    else
    {
      d3dpp.Windowed          = TRUE;
      d3dpp.BackBufferFormat    = D3DFMT_UNKNOWN;
    }

    d3dpp.SwapEffect                = D3DSWAPEFFECT_DISCARD;
    d3dpp.AutoDepthStencilFormat    = D3DFMT_D16;
    d3dpp.EnableAutoDepthStencil    = TRUE;
    d3dpp.AutoDepthStencilFormat    = D3DFMT_D24S8;
    d3dpp.Flags                        = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dpp.PresentationInterval        = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Create the D3DDevice
    m_bIsOk = !FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
                       &d3dpp, &m_pD3DDevice ) );

    if ( !m_bIsOk )
    {
      m_bIsOk = !FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                         &d3dpp, &m_pD3DDevice ) );

      if ( m_bIsOk )
        CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, "RenderManager::D3DCREATE_SOFTWARE_VERTEXPROCESSING" );
    }
    else
      CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING" );

    if ( m_bIsOk )
    {
      // Turn off culling, so we see the front and back of the triangle
      m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
      m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
      m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
      m_pD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
      m_pD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
      m_pD3DDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

      m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
      m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
      m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
      m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );

      m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
      m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
      m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
      m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

      // Turn off D3D lighting, since we are providing our own vertex colors
      m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

      if ( fullscreenMode )
      {
        m_uWidth    = widthScreen;
        m_uHeight    = heightScreen;
      }
      else
        GetWindowRect( hWnd );

      CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",
                                             m_uWidth, m_uHeight );
    }

    if ( !m_bIsOk )
    {
      std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
      CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR, msg_error.c_str() );
      Release();
      throw CException( __FILE__, __LINE__, msg_error );
    }

  }

  D3DXCreateTeapot( m_pD3DDevice, &m_TeapotMesh, 0 );
  return m_bIsOk;
}

void CGraphicsManager::GetWindowRect( HWND hwnd )
{
  RECT rec_window;
  GetClientRect( hwnd, &rec_window );
  m_uWidth = rec_window.right - rec_window.left;
  m_uHeight = rec_window.bottom - rec_window.top;
}

void CGraphicsManager::DrawAxis( float32 Size )
{
  CUSTOMVERTEX v[] =
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

  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, v, sizeof( CUSTOMVERTEX ) );
}

void CGraphicsManager::DrawGrid( float32 Size, Math::CColor Color, int32 GridX, int32 GridZ )
{
  // Check if the size of the grid is null
  if ( Size <= 0 )
    return;

  // Calculate the data
  DWORD grid_color_aux = Color.GetUint32Argb();
  float32 GridXStep = Size / GridX;
  float32 GridZStep = Size / GridZ;
  float32 halfSize = Size * 0.5f;

  // Set the attributes to the paint device
  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );

  // Draw the lines of the X axis
  for ( float32 i = -halfSize; i <= halfSize ; i += GridXStep )
  {
    CUSTOMVERTEX v[] =
    { {i, 0.0f, -halfSize, grid_color_aux}, {i, 0.0f, halfSize, grid_color_aux} };

    m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }

  // Draw the lines of the Z axis
  for ( float32 i = -halfSize; i <= halfSize ; i += GridZStep )
  {
    CUSTOMVERTEX v[] =
    { { -halfSize, 0.0f, i, grid_color_aux}, {halfSize, 0.0f, i, grid_color_aux} };

    m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }
}

void CGraphicsManager::DrawPlane( float32 Size, const Math::Vect3f& normal, float32 distance, Math::CColor Color,
                                  int32 GridX, int32 GridZ )
{

}

void CGraphicsManager::DrawCube( float32 Size )
{
  DrawBox( Size, Size, Size, Math::colWHITE );
}

void CGraphicsManager::DrawCube( float32 Size, Math::CColor Color )
{
  DrawBox( Size, Size, Size, Math::colWHITE );
}

void CGraphicsManager::DrawBox( float32 SizeX, float32 SizeY, float32 SizeZ, Math::CColor Color )
{
  if ( FAILED( D3DXCreateBox( m_pD3DDevice, SizeX, SizeY, SizeZ, &m_BoxMesh, 0 ) ) )
    return;

  CEffectTechnique* EffectTechnique = CEffectManager::GetSingletonPtr()->GetResource( "DefaultTechnique" );

  EffectTechnique->BeginRender();

  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
  UINT l_NumPasses = 0;

  l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );

  if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) ) return;

  for ( UINT b = 0; b < l_NumPasses; ++b )
  {
    l_Effect->BeginPass( b );
    m_BoxMesh->DrawSubset( 0 );
    l_Effect->EndPass();
  }

  l_Effect->End();
  CHECKED_RELEASE( m_BoxMesh );
}

void CGraphicsManager::DrawSphere( float32 Radius, Math::CColor Color, int32 Aristas )
{
  if ( FAILED( D3DXCreateSphere( m_pD3DDevice, Radius, Aristas, Aristas, &m_SphereMesh, 0 ) ) )
    return;

  CEffectTechnique* EffectTechnique = CEffectManager::GetSingletonPtr()->GetResource( "DefaultTechnique" );

  EffectTechnique->BeginRender();

  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
  UINT l_NumPasses = 0;

  l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );

  if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) ) return;

  for ( UINT b = 0; b < l_NumPasses; ++b )
  {
    l_Effect->BeginPass( b );
    m_SphereMesh->DrawSubset( 0 );
    l_Effect->EndPass();
  }

  l_Effect->End();
  CHECKED_RELEASE( m_SphereMesh );
}

void CGraphicsManager::DrawCircle( float32 Radius, Math::CColor Color, int32 Aristas )
{
  DWORD sphere_color_aux = Color.GetUint32Argb();

  float32 Angle = Math::Utils::Deg2Rad( 360.0f / Aristas );

  // Set the attributes to the paint device
  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );

  for ( float32 i = 0; i < Math::two_pi32; i += Angle )
  {
    CUSTOMVERTEX v[2] =
    {
      {
        Math::Utils::Cos( i )* Radius, 0.0f,
        Math::Utils::Sin( i )* Radius,
        0xffffffff
      },
      {
        Math::Utils::Cos( i + Angle )* Radius,
        0.0f,
        Math::Utils::Sin( i + Angle )* Radius,
        0xffffffff
      },
    };

    m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }
}

void CGraphicsManager::DrawLine( const Math::Vect3f& PosA, const Math::Vect3f& PosB, Math::CColor Color )
{
  DWORD color_aux = Color.GetUint32Argb();

  CUSTOMVERTEX v[2] =
  {
    {PosA.x, PosA.y, PosA.z, color_aux},
    {PosB.x, PosB.y, PosB.z, color_aux},
  };

  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
}

void CGraphicsManager::SetTransform( D3DXMATRIX& matrix )
{
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
  CEffectManager::GetSingletonPtr()->SetWorldMatrix( Math::Mat44f( matrix ) );
}

void CGraphicsManager::SetTransform( Math::Mat44f& m )
{
  D3DXMATRIX aux
  (
    m.m00, m.m10, m.m20, m.m30,
    m.m01, m.m11, m.m21, m.m31,
    m.m02, m.m12, m.m22, m.m32,
    m.m03, m.m13, m.m23, m.m33
  );

  m_pD3DDevice->SetTransform( D3DTS_WORLD, &aux );
  CEffectManager::GetSingletonPtr()->SetWorldMatrix( m );
}

//La posicion y el (w,h) esta en pixeles
//La posicion y el (w,h) esta en pixeles
void CGraphicsManager::DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment,
                                   Math::CColor color )
{
  Math::Vect2i finalPos = pos;
  CalculateAlignment( w, h, alignment, finalPos );

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]


  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_COLOR_VERTEX v[4] =
  {
    { ( float32 )finalPos.x, ( float32 )finalPos.y,        0, 1, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) sup_esq.
    ,    { ( float32 )finalPos.x, ( float32 )finalPos.y + h,    0, 1, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) inf_esq.
    , { ( float32 )finalPos.x + w, ( float32 )finalPos.y,        0, 1, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) sup_dr.
    ,    { ( float32 )finalPos.x + w, ( float32 )finalPos.y + h,    0, 1, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v,
                                        sizeof( SCREEN_COLOR_VERTEX ) );
}
void CGraphicsManager::CalculateAlignment( uint32 w, uint32 h, ETypeAlignment alignment, Math::Vect2i& finalPos )
{
  switch ( alignment )
  {
  case CENTER:
  {
    finalPos.x -= ( uint32 )( w * 0.5f );
    finalPos.y -= ( uint32 )( h * 0.5f );
  }
  break;

  case UPPER_LEFT:
  {
    //Por defecto ya est alienado de esta manera :)
  }
  break;

  case UPPER_RIGHT:
  {
    finalPos.x -= w;
  }
  break;

  case LOWER_RIGHT:
  {
    finalPos.x -= w;
    finalPos.y -= h;
  }
  break;

  case LOWER_LEFT:
  {
    finalPos.y -= h;
  }
  break;

  default:
  {
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
                                           "RenderManager:: Se est� intentado renderizar un quad2d con una alineacion desconocida" );
  }
  break;
  }
}

void CGraphicsManager::EnableAlphaBlend()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  // render el quad de difuminacion....

  m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  //// render el quad de difuminacion....
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}

void CGraphicsManager::DisableAlphaBlend()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void CGraphicsManager::EnableZBuffering()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
}

void CGraphicsManager::DisableZBuffering()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void CGraphicsManager::GetRay( const Math::Vect2i& mousePos, Math::Vect3f& posRay, Math::Vect3f& dirRay )
{
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;

  m_pD3DDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );
  m_pD3DDevice->GetTransform( D3DTS_VIEW, &viewMatrix );
  m_pD3DDevice->GetTransform( D3DTS_WORLD, &worldMatrix );

  float32 angle_x = ( ( ( 2.0f * mousePos.x ) / m_uWidth ) - 1.0f ) / projectionMatrix( 0, 0 );
  float32 angle_y = ( ( ( -2.0f * mousePos.y ) / m_uHeight ) + 1.0f ) / projectionMatrix( 1, 1 );

  D3DXVECTOR3 ray_org = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
  D3DXVECTOR3 ray_dir = D3DXVECTOR3( angle_x, angle_y, 1.0f );

  D3DXMATRIX m = worldMatrix * viewMatrix;
  D3DXMatrixInverse( &worldViewInverse, 0, &m );
  D3DXVec3TransformCoord( &ray_org, &ray_org, &worldViewInverse );
  D3DXVec3TransformNormal( &ray_dir, &ray_dir, &worldViewInverse );
  D3DXVec3Normalize( &ray_dir, &ray_dir );

  posRay.x = ray_org.x;
  posRay.y = ray_org.y;
  posRay.z = ray_org.z;

  dirRay.x = ray_dir.x;
  dirRay.y = ray_dir.y;
  dirRay.z = ray_dir.z;
}

void CGraphicsManager::DrawRectangle2D( const Math::Vect2i& pos, uint32 w, uint32 h, Math::CColor& backGroundColor,
                                        uint32 edge_w, uint32 edge_h, Math::CColor& edgeColor )
{
  //Draw background quad2D:
  DrawQuad2D( pos, w, h, UPPER_LEFT, backGroundColor );

  //Draw the four edges:

  //2 Horizontal:
  Math::Vect2i pos_aux = pos;
  pos_aux.y -= edge_h;
  DrawQuad2D( pos_aux, w, edge_h, UPPER_LEFT, edgeColor );
  pos_aux = pos;
  pos_aux.y += h;
  DrawQuad2D( pos_aux, w, edge_h, UPPER_LEFT, edgeColor );

  //2 Vertical:
  pos_aux = pos;
  pos_aux.x -= edge_w;
  pos_aux.y -= edge_h;
  DrawQuad2D( pos_aux, edge_w, h + ( 2 * edge_w ), UPPER_LEFT, edgeColor );
  pos_aux.x = pos.x + w;
  DrawQuad2D( pos_aux, edge_w, h + ( 2 * edge_w ), UPPER_LEFT, edgeColor );
}

void CGraphicsManager::GetWidthAndHeight( uint32& w, uint32& h )
{
  w = m_uWidth;
  h = m_uHeight;
}

void CGraphicsManager::DrawCylinder( float32 Top_Radius, float32 Bottom_Radius, float32 h, uint32 Aristas,
                                     Math::CColor Color, bool drawCover )
{
  if ( FAILED( D3DXCreateCylinder( m_pD3DDevice, Top_Radius, Bottom_Radius, h, Aristas, Aristas, &m_CylinderMesh, 0 ) ) )
    return;

  m_CylinderMesh->DrawSubset( 0 );

  CHECKED_RELEASE( m_CylinderMesh );
}

void CGraphicsManager::DrawCapsule( float32 radius, float32 h, uint32 Aristas, Math::CColor Color )
{
  D3DXMATRIX matrix;
  D3DXMatrixIdentity( &matrix );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );

  DrawCylinder( radius, radius, h, Aristas, Color, true );

  D3DXMATRIX translation1, translation2;
  D3DXMatrixTranslation( &translation1, 0.f, h * 0.5f, 0.f );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &translation1 );
  //DrawSphere(radius, Color, Aristas, mode, HALF_TOP);

  D3DXMatrixTranslation( &translation2, 0.f, -h * 0.5f, 0.f );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &translation2 );
  //DrawSphere(radius, Color, Aristas, mode, HALF_BOTTOM);
}

void CGraphicsManager::DrawQuad3D( const Math::Vect3f& pos, const Math::Vect3f& up, const Math::Vect3f& right,
                                   float32 w, float32 h, Math::CColor color )
{
  Math::Vect3f upper_left, upper_right, down_right, down_left;
  upper_left    = pos + up * h * 0.5f - right * w * 0.5f;
  upper_right    = pos + up * h * 0.5f + right * w * 0.5f;
  down_left        = pos - up * h * 0.5f - right * w * 0.5f;
  down_right    = pos - up * h * 0.5f + right * w * 0.5f;

  DrawQuad3D( upper_left, upper_right, down_left, down_right, color );
}

void CGraphicsManager::DrawQuad3D( const Math::Vect3f& ul, const Math::Vect3f& ur, const Math::Vect3f& dl,
                                   const Math::Vect3f& dr, Math::CColor color )
{
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  CUSTOMVERTEX v[4] =
  {
    { ul.x,        ul.y,        ul.z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) up_left.
    , { dl.x,        dl.y,        dl.z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) down_left.
    ,    { ur.x,        ur.y,        ur.z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) up_right.
    ,    { dr.x,        dr.y,        dr.z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )} //(x,y) down_right.
  };

  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( CUSTOMVERTEX ) );
}

void CGraphicsManager::DrawIcoSphere()
{
  const float32 X = 0.525731112119133606f;
  const float32 Z = 0.850650808352039932f;

  unsigned short indices[] =
  {
    0, 4, 1, 0, 9, 4 , 9, 5, 4, 4, 5, 8, 4, 8, 1,
    8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 2, 7, 3,
    7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6 , 0, 1, 6,
    6, 1, 10, 9, 0, 11, 9, 11, 2, 9, 2, 5  , 7, 2, 11
  };

  Math::CColor color = Math::colBLACK;

  CUSTOMVERTEX v[] =
  {
    { -X, 0.0, Z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {X, 0.0, Z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    { -X, 0.0, -Z,    D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {X, 0.0, -Z,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {0.0, Z, X,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {0.0, Z, -X,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {0.0, -Z, X,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {0.0, -Z, -X,    D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {Z, X, 0.0,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    { -Z, X, 0.0,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    {Z, -X, 0.0,        D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )},
    { -Z, -X, 0.0,    D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() )}
  };

  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  m_pD3DDevice->SetTexture( 0, NULL );
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 12, 20, indices, D3DFMT_INDEX16, v,
                                        sizeof( CUSTOMVERTEX ) );
}

void CGraphicsManager::RenderCursor()
{
  D3DXMATRIX mat;
  D3DXMatrixOrthoRH( &mat, ( float )m_uWidth, ( float )m_uHeight, -1.0f, 1.0f );
  m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &mat );

  D3DXMatrixIdentity( &mat );
  m_pD3DDevice->SetTransform( D3DTS_VIEW, &mat );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &mat );

  VERTEX2 Vertex[3];

  Math::Vect2i l_MousePosition;
  CInputManager::GetSingletonPtr()->GetPosition( IDV_MOUSE, l_MousePosition );

  Vertex[0].pos = D3DXVECTOR3( ( float )( l_MousePosition.x ), ( float )( l_MousePosition.y ), 0.0f );
  Vertex[0].d = 0xFFFF0000;

  Vertex[2].pos = D3DXVECTOR3( ( float )( l_MousePosition.x ), ( float )( l_MousePosition.y ), 0.0f );
  Vertex[2].d = 0xFFFF0000;

  Vertex[1].pos = D3DXVECTOR3( ( float )( l_MousePosition.x ), ( float )( l_MousePosition.y ), 0.0f );
  Vertex[1].d = 0xFFFF0000;

  m_pD3DDevice->SetFVF( VERTEX2::getFlags() );

  m_pD3DDevice->SetTexture( 0, NULL );

  m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

  m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
  m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

  m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1, ( void* ) Vertex, sizeof( VERTEX2 ) );

  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

  m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}


void CGraphicsManager::EnableZTest()
{
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void CGraphicsManager::DisableZTest()
{
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
}

void CGraphicsManager::EnableZWrite()
{
  m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}

void CGraphicsManager::DisableZWrite()
{
  m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
}

void CGraphicsManager::Present()
{
  m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CGraphicsManager::DrawTeapot()
{
  CEffectTechnique* EffectTechnique = CEffectManager::GetSingletonPtr()->GetResource( "DefaultTechnique" );

  EffectTechnique->BeginRender();

  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
  UINT l_NumPasses = 0;

  l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );

  if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) ) return;

  for ( UINT b = 0; b < l_NumPasses; ++b )
  {
    l_Effect->BeginPass( b );
    m_TeapotMesh->DrawSubset( 0 );
    l_Effect->EndPass();
  }

  l_Effect->End();
}