#include "GraphicsManager.h"
#include "Utils\Defines.h"
#include "Math\MathTypes.h"
#include "Logger\Logger.h"
#include "Cameras\CameraManager.h"
#include "Cameras\Camera.h"
#include "Math\Vector3.h"
#include "EngineManagers.h"
#include <malloc.h>
#include <vector>
#include "Exceptions\Exception.h"
#include "InputManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"

#include "RenderableVertex/VertexTypes.h"
#include "Texture/Texture.h"
#include "Fonts/FontManager.h"
#include "EngineConfig.h"

#include "Shapes/Shapes.h"

#include "Effects\SharedEffect.h"

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

struct VERTEX2
{
  D3DXVECTOR3    pos;
  DWORD       d;

  static unsigned int getFlags()
  {
    return ( D3DFVF_XYZ | D3DFVF_DIFFUSE );
  }
};

CGraphicsManager::CGraphicsManager( const CXMLTreeNode& atts )
  : CManager( atts )
  , mWidth( 0 )
  , mHeight( 0 )
  , m_BackbufferColor_debug( Math::colBLUE )
  , m_BackbufferColor_release( Math::colBLACK )
  , m_bPaintSolid( true )
  , m_TeapotMesh( 0 )
  , m_CylinderMesh( 0 )
  , m_SphereMesh( 0 )
  , m_BoxMesh( 0 )
  , m_IBQuad( 0 )
  , m_VBQuad( 0 )
  , mDirectXDevice( 0 )
  , mDirectXObject( 0 )
{
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

bool CGraphicsManager::isDeviceLost()
{
    HRESULT l_Result = mDirectXDevice->TestCooperativeLevel();

    /************************************************************************/
    /* Device lost and can't be reseted yet                                 */
    /************************************************************************/
    if(l_Result == D3DERR_DEVICELOST)
    {
        Sleep(20);
        return true;
    }

    /************************************************************************/
    /* Device lost and can be reseted                                       */
    /************************************************************************/
    if(l_Result == D3DERR_DEVICENOTRESET)
    {
        return true;
    }

    /************************************************************************/
    /* Device is OK                                                         */
    /************************************************************************/
    if(l_Result == D3D_OK)
    {
        return false;
    }

    /************************************************************************/
    /* Fatal error, internal driver error, exit the game                    */
    /************************************************************************/
    if(l_Result == D3DERR_DRIVERINTERNALERROR)
    {
        PostQuitMessage(1);
    }

    return false;
}

bool CGraphicsManager::canDeviceBeReseted()
{
    /************************************************************************/
    /* Device lost and can be reseted                                       */
    /************************************************************************/
    return mDirectXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
}

void CGraphicsManager::Release()
{
  CHECKED_RELEASE( m_SphereMesh );
  CHECKED_RELEASE( m_BoxMesh );
  CHECKED_RELEASE( m_CylinderMesh );
  CHECKED_RELEASE( m_TeapotMesh );

  DestroyShapes();
}

void CGraphicsManager::BeginScene()
{
  HRESULT hr = mDirectXDevice->BeginScene();
  assert( SUCCEEDED( hr ) );

  mDirectXDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  mDirectXDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

  if ( m_bPaintSolid )
    mDirectXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
  else
    mDirectXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

  EffectManagerInstance->BeginRender();
}

void CGraphicsManager::BeginRender()
{
  // Begin the scene
  HRESULT hr = mDirectXDevice->BeginScene();
  //assert( SUCCEEDED( hr ) );
  mDirectXDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  mDirectXDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

  if ( m_bPaintSolid )
    mDirectXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
  else
    mDirectXDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );


}

void CGraphicsManager::Clear()
{
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
  uint32 red        = ( uint32 )( m_BackbufferColor_debug.GetRed() * 255 );
  uint32 green      = ( uint32 )( m_BackbufferColor_debug.GetGreen() * 255 );
  uint32 blue       = ( uint32 )( m_BackbufferColor_debug.GetBlue() * 255 );
  mDirectXDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
  uint32 red      = ( uint32 )( m_BackbufferColor_release.GetRed() * 255 );
  uint32 green    = ( uint32 )( m_BackbufferColor_release.GetGreen() * 255 );
  uint32 blue     = ( uint32 )( m_BackbufferColor_release.GetBlue() * 255 );
  mDirectXDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( red, green, blue ), 1.0f, 0 );
#endif
}

void CGraphicsManager::Clear( bool target, bool zbuffer, bool stencil, u_int mask )
{
  u_int l_Mask = 0x00000000;

  if ( target )
    l_Mask = l_Mask | D3DCLEAR_TARGET;

  if ( zbuffer )
    l_Mask = l_Mask | D3DCLEAR_ZBUFFER;

  if ( stencil )
    l_Mask = l_Mask | D3DCLEAR_STENCIL;

  mDirectXDevice->Clear( 0, NULL, l_Mask, mask, 1.0f, 0 );
}

void CGraphicsManager::EndScene()
{
  mDirectXDevice->EndScene();
}

void CGraphicsManager::EndRender()
{
  mDirectXDevice->EndScene();
  // Present the backbuffer contents to the display
  //mDirectXDevice->Present( NULL, NULL, NULL, NULL );
}

void CGraphicsManager::SetupMatrices()
{
  Math::Vect3f l_CameraPosition;
  D3DXMATRIX l_matView;
  D3DXMATRIX l_matProject;
  // Obtain the current camera from the camera manager
  CCamera* l_CurrentCamera = CameraMInstance->GetCurrentCamera();

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
    l_CameraPosition = l_CurrentCamera->GetPosition();
    l_matView        = l_CurrentCamera->GetMatrixView();
    l_matProject     = l_CurrentCamera->GetMatrixProj();
    l_CurrentCamera->UpdateFrustum( l_matView * l_matProject );
  }

  mDirectXDevice->SetTransform( D3DTS_VIEW, &l_matView );
  mDirectXDevice->SetTransform( D3DTS_PROJECTION, &l_matProject );
  // Set the new parameters to the effect manager
  EffectManagerInstance->ActivateCamera( Math::Mat44f( l_matView ), Math::Mat44f( l_matProject ), l_CameraPosition );
}

void CGraphicsManager::Init()
{
  m_WindowId = EngineConfigInstance->GetWindowId();
  Math::Vect2i lScreenResolution = EngineConfigInstance->GetScreenResolution();

  mWidth = lScreenResolution.x;
  mHeight = lScreenResolution.y;

  mDirectXObject = Direct3DCreate9( D3D_SDK_VERSION );

  if ( mDirectXObject == NULL )
  {
    LOG_ERROR_APPLICATION( "Error of Direct3DCreate9( D3D_SDK_VERSION )" );
    return;
  }

  // Choose a way of creating the device from the configuration of the engine
  mOk = ( EngineConfigInstance->GetFullScreenMode() ) ? CreateFullScreenMode( EngineConfigInstance ) : CreateWindowedMode( EngineConfigInstance );

  if ( !mOk )
    FATAL_ERROR( "GraphicsManager::The DirectX has not been init!!!" );

  LOG_INFO_APPLICATION( "Init of Graphics Manager Ok" );
  // Turn off culling, so we see the front and back of the triangle
  mDirectXDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
  mDirectXDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
  mDirectXDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  // Turn off D3D lighting, since we are providing our own vertex colors
  mDirectXDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  Math::Vect2i lWindowPosition    = EngineConfigInstance->GetScreenPosition();
  Math::Vect2i lWindowSize        = EngineConfigInstance->GetScreenSize();
  Math::Vect2i lWindowResolution  = EngineConfigInstance->GetScreenResolution();
  LOG_INFO_APPLICATION( "GraphicsManager::Window created in (%d,%d) with size (%d,%d) and resolution(%d,%d)",
                        lWindowPosition.x, lWindowPosition.y,
                        lWindowSize.x, lWindowSize.y,
                        lWindowResolution.x, lWindowResolution.y );

  //
  // Create the default meshes
  //
  if ( FAILED( D3DXCreateTeapot( mDirectXDevice, &m_TeapotMesh, 0 ) ) )
    LOG_ERROR_APPLICATION( "GraphicsManager::Error creating the teapot" );

  if ( D3DXCreateSphere( mDirectXDevice, 1.0f, 10, 10, &m_SphereMesh, 0 ) )
    LOG_ERROR_APPLICATION( "GraphicsManager::Error creating the sphere" );

  if ( FAILED( D3DXCreateBox( mDirectXDevice, 1.0f, 1.0f, 1.0f, &m_BoxMesh, 0 ) ) )
    LOG_ERROR_APPLICATION( "GraphicsManager::Error creating the box" );

  if ( FAILED( D3DXCreateCylinder( mDirectXDevice, 1.0f, 1.0f, 1.0f, 10, 10, &m_CylinderMesh, 0 ) ) )
    LOG_ERROR_APPLICATION( "GraphicsManager::Error creating the cylinder" );

  InitShapes();
}

DWORD CGraphicsManager::GetBehaviorFlags()
{
  DWORD dwBehaviorFlags = 0;
  //
  // Do we support hardware vertex processing? if so, use it.
  // If not, downgrade to software.
  //
  D3DCAPS9 d3dCaps;

  if ( FAILED( mDirectXObject->GetDeviceCaps( D3DADAPTER_DEFAULT,
               D3DDEVTYPE_HAL, &d3dCaps ) ) )
  {
    LOG_ERROR_APPLICATION( "GraphicsManager::Error getting the device caps" );
    return dwBehaviorFlags;
  }

  if ( d3dCaps.VertexProcessingCaps != 0 )
  {
    dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
    LOG_INFO_APPLICATION( "GraphicsManager::D3DCREATE_HARDWARE_VERTEXPROCESSING" );
  }
  else
  {
    dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    LOG_INFO_APPLICATION( "GraphicsManager::D3DCREATE_SOFTWARE_VERTEXPROCESSING" );
  }

  return dwBehaviorFlags;
}

bool CGraphicsManager::CreateFullScreenMode( CEngineConfig* aEngineConfig )
{
  //
  // For the default adapter, examine all of its display modes to see if any
  // of them can give us the hardware support we desire.
  //
  int nMode = 0;
  D3DDISPLAYMODE d3ddm;
  bool bDesiredAdapterModeFound = false;
  Math::Vect2i lScreenResolution = aEngineConfig->GetScreenResolution();
  int nMaxAdapterModes = mDirectXObject->GetAdapterModeCount( D3DADAPTER_DEFAULT,
                         D3DFMT_X8R8G8B8 );

  for ( nMode = 0; nMode < nMaxAdapterModes; ++nMode )
  {
    if ( FAILED( mDirectXObject->EnumAdapterModes( D3DADAPTER_DEFAULT,
                 D3DFMT_X8R8G8B8, nMode, &d3ddm ) ) )
    {
      LOG_ERROR_APPLICATION( "GraphicsManager::Error of AdapterModes" );
      return false;
    }

    // Does this adapter mode support a mode of the given configuration
    Math::Vect2i lScreenResolution = aEngineConfig->GetScreenResolution();

    if ( d3ddm.Width != lScreenResolution.x || d3ddm.Height != lScreenResolution.y )
      continue;

    // Does this adapter mode support a 32-bit RGB pixel format?
    if ( d3ddm.Format != D3DFMT_X8R8G8B8 )
      continue;

    // We found a match!
    bDesiredAdapterModeFound = true;
    break;
  }

  if ( bDesiredAdapterModeFound == false )
  {
    LOG_ERROR_APPLICATION( "GraphicsManager::The adapter does not have a Format of D3DFMT_X8R8G8B8, and a resolution of (%d,%d)",
                           lScreenResolution.x, lScreenResolution.y );
    return false;
  }

  //
  // Here's the manual way of verifying hardware support.
  //

  // Can we get a 32-bit back buffer?
  if ( FAILED( mDirectXObject->CheckDeviceType( D3DADAPTER_DEFAULT,
               D3DDEVTYPE_HAL,
               D3DFMT_X8R8G8B8,
               D3DFMT_X8R8G8B8,
               FALSE ) ) )
  {
    LOG_ERROR_APPLICATION( "GraphicsManager::This adapter has no support for  a 32 bit back buffer " );
    return false;
  }

  // Can we get a z-buffer that's at least 16 bits?
  if ( FAILED( mDirectXObject->CheckDeviceFormat( D3DADAPTER_DEFAULT,
               D3DDEVTYPE_HAL,
               D3DFMT_X8R8G8B8,
               D3DUSAGE_DEPTHSTENCIL,
               D3DRTYPE_SURFACE,
               D3DFMT_D16 ) ) )
  {
    LOG_ERROR_APPLICATION( "GraphicsManager::This adapter has no support for a 16 bit back buffer " );
    return false;
  }

  // Get the flags of the behaviour
  DWORD dwBehaviorFlags = GetBehaviorFlags();
  //
  // Everything checks out - create a simple, full-screen device.
  //
  D3DPRESENT_PARAMETERS d3dpp;
  memset( &d3dpp, 0, sizeof( d3dpp ) );
  d3dpp.hDeviceWindow               = m_WindowId;
  d3dpp.Windowed                    = FALSE;
  d3dpp.EnableAutoDepthStencil      = TRUE;
  d3dpp.AutoDepthStencilFormat      = D3DFMT_D24S8;
  d3dpp.SwapEffect                  = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferCount             = 1;
  d3dpp.BackBufferWidth             = lScreenResolution.x;
  d3dpp.BackBufferHeight            = lScreenResolution.y;
  d3dpp.BackBufferFormat            = D3DFMT_X8R8G8B8;
  d3dpp.PresentationInterval        = D3DPRESENT_INTERVAL_ONE;
  d3dpp.Flags                       = 0;//D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
  d3dpp.FullScreen_RefreshRateInHz  = D3DPRESENT_RATE_DEFAULT;
  d3dpp.MultiSampleType             = D3DMULTISAMPLE_NONE;
  d3dpp.MultiSampleQuality          = 0;

  HRESULT hr = mDirectXObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_WindowId, dwBehaviorFlags, &d3dpp,
               &mDirectXDevice );

  if ( FAILED( hr ) )
  {
    LOG_ERROR_APPLICATION( "GraphicsManager::Error creating the device" );
    return false;
  }

  return true;
}

bool CGraphicsManager::CreateWindowedMode( CEngineConfig* aEngineConfig )
{
  D3DDISPLAYMODE d3ddm;

  if ( FAILED( mDirectXObject->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
  {
    // TO DO: Respond to failure of GetAdapterDisplayMode
    return false;
  }

  HRESULT hr;

  if ( FAILED( hr = mDirectXObject->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                    d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
                    D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
  {
    if ( hr == D3DERR_NOTAVAILABLE )
      // POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
      return false;
  }

  // Get the flags of the behaviour
  DWORD dwBehaviorFlags = GetBehaviorFlags();
  //
  // Everything checks out - create a simple, windowed device.
  //
  D3DPRESENT_PARAMETERS d3dpp;
  memset( &d3dpp, 0, sizeof( d3dpp ) );
  d3dpp.BackBufferFormat       = d3ddm.Format;
  d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  d3dpp.Windowed               = TRUE;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

  if ( FAILED( mDirectXObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_WindowId,
               dwBehaviorFlags, &d3dpp, &mDirectXDevice ) ) )
  {
    LOG_ERROR_APPLICATION( "Error creating the device" );
    return false;
  }

  return true;
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
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  mDirectXDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, v, sizeof( CUSTOMVERTEX ) );
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
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );

  // Draw the lines of the X axis
  for ( float32 i = -halfSize; i <= halfSize ; i += GridXStep )
  {
    CUSTOMVERTEX v[] =
    { {i, 0.0f, -halfSize, grid_color_aux}, {i, 0.0f, halfSize, grid_color_aux} };
    mDirectXDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }

  // Draw the lines of the Z axis
  for ( float32 i = -halfSize; i <= halfSize ; i += GridZStep )
  {
    CUSTOMVERTEX v[] =
    { { -halfSize, 0.0f, i, grid_color_aux}, {halfSize, 0.0f, i, grid_color_aux} };
    mDirectXDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }
}

void CGraphicsManager::DrawPlane( float32 size, const Math::Vect3f& normal, float32 distance,
                                  Math::CColor Color,
                                  int32 GridX, int32 GridZ )
{
  D3DXMATRIX matrix;
  D3DXMatrixIdentity( &matrix );
  mDirectXDevice->SetTransform( D3DTS_WORLD, &matrix );
  float A, B, C, D;
  A = normal.x;
  B = normal.y;
  C = normal.z;
  D = distance;
  Math::Vect3f pointA, pointB;

  if ( C != 0 )
  {
    pointA = Math::Vect3f( 0.f, 0.f, -D / C );
    pointB = Math::Vect3f( 1.f, 1.f, ( D - A - B ) / C );
  }
  else if ( B != 0 )
  {
    pointA = Math::Vect3f( 0.f, -D / B, 0.f );
    pointB = Math::Vect3f( 1.f, ( -D - A - C ) / B, 1.f );
  }
  else if ( A != 0 )
  {
    pointA = Math::Vect3f( -D / A, 0.f, 0.f );
    pointB = Math::Vect3f( ( -D - B - C ) / A, 1.f, 1.f );
  }
  else
  {
    LOG_ERROR_APPLICATION( "drawplane : ALL VALUES = 0" );
    std::string l_msgerror = "Error, se genero un logger con la informacion";
    throw CException( __FILE__, __LINE__, l_msgerror );
  }

  Math::Vect3f vectorA = pointB - pointA;
  vectorA.Normalize();
  Math::Vect3f vectorB;
  vectorB = normal ^ vectorA;
  vectorB.Normalize();
  Math::Vect3f initPoint = normal * distance;
  assert( GridX > 0 );
  assert( GridZ > 0 );
  //LINEAS EN Z
  Math::Vect3f initPointA = initPoint - vectorB * size * 0.5;

  for ( int b = 0; b <= GridX; ++b )
  {
    DrawLine( initPointA + vectorA * size * 0.5, initPointA - vectorA * size * 0.5, Color );
    initPointA += vectorB * size / ( float )GridX;
  }

  initPointA = initPoint - vectorA * size * 0.5;

  for ( int b = 0; b <= GridX; ++b )
  {
    DrawLine( initPointA + vectorB * size * 0.5, initPointA - vectorB * size * 0.5, Color );
    initPointA += vectorA * size / ( float )GridX;
  }
}

void CGraphicsManager::DrawCube( const Math::Vect3f& aPosition, float32 Size,
                                 const Math::CColor aColor )
{
  DrawBox( aPosition, Size, Size, Size, aColor );
}

void CGraphicsManager::DrawCube( const Math::Mat44f& aTransform, float32 Size,
                                 const Math::CColor aColor )
{
  DrawBox( aTransform, Size, Size, Size, aColor );
}

void CGraphicsManager::DrawBox( const Math::Vect3f& aPosition, float32 SizeX, float32 SizeY, float32 SizeZ,
                                Math::CColor Color )
{
  Math::Mat44f transform;
  transform.Translate( aPosition );
  DrawBox( transform, SizeX, SizeY, SizeZ, Color );
}

void CGraphicsManager::DrawBox( const Math::Mat44f& aTransform, float32 SizeX, float32 SizeY, float32 SizeZ,
                                Math::CColor Color )
{
  Math::Mat44f lTransform;
  lTransform.Scale( SizeX, SizeY, SizeZ );
  RenderMesh( aTransform * lTransform, m_BoxMesh, Color );
}

void CGraphicsManager::DrawSphere( const Math::Vect3f& aPosition, float32 Radius, Math::CColor Color, int32 Aristas )
{
  Math::Mat44f transform;
  transform.Translate( aPosition );
  transform.Scale( Radius, Radius, Radius );
  DrawSphere( transform, Radius, Color, Aristas );
}

void CGraphicsManager::DrawSphere( const Math::Mat44f& aTransform, float32 Radius, Math::CColor Color, int32 Aristas )
{
  Math::Mat44f lTransform;
  lTransform.Scale( Radius, Radius, Radius );
  RenderMesh( aTransform * lTransform, m_SphereMesh, Color );
}
void CGraphicsManager::DrawCircle( float32 Radius, Math::CColor Color, int32 Aristas )
{
  DWORD sphere_color_aux = Color.GetUint32Argb();
  float32 Angle = Math::Utils::Deg2Rad( 360.0f / Aristas );
  // Set the attributes to the paint device
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );

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
    mDirectXDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
  }
}
void CGraphicsManager::DrawLine( const Math::Vect3f& PosA, const Math::Vect3f& PosB,
                                 Math::CColor Color )
{
  /*
      Math::Vect3f lDirectionCylinder( 0.0f, 0.0f, 1.0f );
      Math::Vect3f lDirectionLine =  PosA - PosB;
      float32 lDistance = lDirectionLine.Length();
      lDirectionLine.Normalize();
      float32 lAngle = Math::Utils::ACos( lDirectionCylinder.DotProduct( lDirectionLine ) );
      Math::Mat44f m;
      m.Translate( PosA );
      Math::Mat44f m2;
      m2.Translate( Math::Vect3f( 0.0f, 0.0f, lDistance * 0.5 ) );
      SetTransform( m );
      DrawCylinder( 0.01, 0.01, ( PosA - PosB ).Length(), 10, Color, true );
      m.SetIdentity();
      SetTransform( m2 * m );
  */
  DWORD color_aux = Color.GetUint32Argb();
  CUSTOMVERTEX v[2] =
  {
    {PosA.x, PosA.y, PosA.z, color_aux},
    {PosB.x, PosB.y, PosB.z, color_aux},
  };
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  mDirectXDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof( CUSTOMVERTEX ) );
}
void CGraphicsManager::SetTransform( D3DXMATRIX& matrix )
{
  mDirectXDevice->SetTransform( D3DTS_WORLD, &matrix );
  EffectManagerInstance->SetWorldMatrix( Math::Mat44f( matrix ) );
}
void CGraphicsManager::SetTransform( const Math::Mat44f& m )
{
  D3DXMATRIX aux
  (
    m.m00, m.m10, m.m20, m.m30,
    m.m01, m.m11, m.m21, m.m31,
    m.m02, m.m12, m.m22, m.m32,
    m.m03, m.m13, m.m23, m.m33
  );
  mDirectXDevice->SetTransform( D3DTS_WORLD, &aux );
  EffectManagerInstance->SetWorldMatrix( m );
}
//La posicion y el (w,h) esta en pixeles
//La posicion y el (w,h) esta en pixeles
void CGraphicsManager::DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                                   ETypeAlignment alignment,
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
  mDirectXDevice->SetFVF( SCREEN_COLOR_VERTEX::GetFVF() );
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v,
                                          sizeof( SCREEN_COLOR_VERTEX ) );
}
void CGraphicsManager::CalculateAlignment( uint32 w, uint32 h, ETypeAlignment alignment,
    Math::Vect2i& finalPos )
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

  case UPPER_MIDDLE:
  {
    finalPos.x -= ( uint32 )( w * 0.5f );
  }
  break;

  case LOWER_MIDDLE:
  {
    finalPos.x -= ( uint32 )( w * 0.5f );
    finalPos.y -= h;
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
    LOG_ERROR_APPLICATION( "The Quad 2d is has an unknown align" );
  }

  break;
  }
}
void CGraphicsManager::EnableAlphaBlend()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  //// render el quad de difuminacion....
  mDirectXDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  mDirectXDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  mDirectXDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}
void CGraphicsManager::DisableAlphaBlend()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}
void CGraphicsManager::EnableZBuffering()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
}
void CGraphicsManager::DisableZBuffering()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}
void CGraphicsManager::GetRay( const Math::Vect2i& mousePos, Math::Vect3f& posRay,
                               Math::Vect3f& dirRay )
{
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;
  mDirectXDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );
  mDirectXDevice->GetTransform( D3DTS_VIEW, &viewMatrix );
  mDirectXDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
  float angle_x = ( ( ( 2.0f * mousePos.x ) / 392 ) - 1.0f );
  float angle_y = ( ( ( -2.0f * mousePos.y ) / 281 ) + 1.0f );
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
void CGraphicsManager::DrawRectangle2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                                        Math::CColor& backGroundColor,
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
  w = mWidth;
  h = mHeight;
}

void CGraphicsManager::DrawCylinder( const Math::Mat44f& aTransform, float32 Top_Radius, float32 Bottom_Radius,
                                     float32 h, uint32 Aristas, Math::CColor Color, bool drawCover )
{
  Math::Mat44f lTransform;
  lTransform.Scale( 0, Top_Radius, h );
  RenderMesh( aTransform * lTransform, m_CylinderMesh, Color );
}

void CGraphicsManager::DrawCylinder( const Math::Vect3f& aPosition, float32 Top_Radius, float32 Bottom_Radius,
                                     float32 h, uint32 Aristas, Math::CColor Color, bool drawCover )
{
  Math::Mat44f transform;
  transform.Translate( aPosition );
  transform.Scale( 0, Top_Radius, h );
  DrawCylinder( transform, Top_Radius, Bottom_Radius, h, Aristas, Color, drawCover );
}

void CGraphicsManager::DrawCapsule( const Math::Vect3f& aPosition, float32 radius, float32 h, uint32 Aristas,
                                    Math::CColor Color )
{
  Math::Mat44f t;
  t.RotByAngleX( Math::half_pi32 );
  SetTransform( t );
  DrawCylinder( aPosition, radius, radius, h, Aristas, Color, false );
  t.SetIdentity();
  t.Translate( Math::Vect3f( 0, h * 0.5f, 0 ) );
  SetTransform( t );
  DrawSphere( aPosition, radius, Color, Aristas );
  t.SetIdentity();
  t.Translate( Math::Vect3f( 0, -h * 0.5f, 0 ) );
  SetTransform( t );
  DrawSphere( aPosition, radius, Color, Aristas );
  t.SetIdentity();
  SetTransform( t );
}
void CGraphicsManager::DrawQuad3D( const Math::Vect3f& pos, const Math::Vect3f& up,
                                   const Math::Vect3f& right,
                                   float32 w, float32 h, Math::CColor color )
{
  Math::Vect3f upper_left, upper_right, down_right, down_left;
  upper_left    = pos + up * h * 0.5f - right * w * 0.5f;
  upper_right    = pos + up * h * 0.5f + right * w * 0.5f;
  down_left        = pos - up * h * 0.5f - right * w * 0.5f;
  down_right    = pos - up * h * 0.5f + right * w * 0.5f;
  DrawQuad3D( upper_left, upper_right, down_left, down_right, color );
}
void CGraphicsManager::DrawQuad3D( const Math::Vect3f& ul, const Math::Vect3f& ur,
                                   const Math::Vect3f& dl,
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
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v,
                                          sizeof( CUSTOMVERTEX ) );
}
void CGraphicsManager::DrawQuad3DWithTechnique( const Math::Vect3f& ul, const Math::Vect3f& ur,
    const Math::Vect3f& dl,
    const Math::Vect3f& dr, const Math::Vect3f& n, CEffectTechnique* EffectTechnique,
    CTexture* Texture )
{
  // TODO
  if ( EffectTechnique == NULL )
    return;

  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();

  if ( l_Effect != NULL )
  {
    mDirectXDevice->SetVertexDeclaration( TNORMAL_T1_VERTEX::GetVertexDeclaration() );
    l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
    UINT l_NumPasses;
    l_Effect->Begin( &l_NumPasses, 0 );

    for ( UINT iPass = 0; iPass < l_NumPasses; iPass++ )
    {
      l_Effect->BeginPass( iPass );
      TNORMAL_T1_VERTEX v[4] =
      {
        { ul.x, ul.y, ul.z, n.x, n.y, n.z, 0, 1},
        { dl.x, dl.y, dl.z, n.x, n.y, n.z, 0, 0},
        { ur.x, ur.y, ur.z, n.x, n.y, n.z, 1, 1},
        { dr.x, dr.y, dr.z, n.x, n.y, n.z, 1, 0}
      };
      mDirectXDevice->SetFVF( TNORMAL_T1_VERTEX::GetFVF() );
      mDirectXDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

      if ( Texture != NULL )
        Texture->Activate( 0 );

      mDirectXDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( TNORMAL_T1_VERTEX ) );
      l_Effect->EndPass();
    }

    l_Effect->End();
  }
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
  mDirectXDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  mDirectXDevice->SetTexture( 0, NULL );
  mDirectXDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 12, 20, indices, D3DFMT_INDEX16, v,
                                          sizeof( CUSTOMVERTEX ) );
}

void CGraphicsManager::EnableAlphaTest()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
}
void CGraphicsManager::DisableAlphaTest()
{
  mDirectXDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
}
void CGraphicsManager::SetBlendOP( const std::string& BlendOPState )
{
  mDirectXDevice->SetRenderState( D3DRS_BLENDOP, ToD3DBlendOPEnum( BlendOPState ) );
}
void CGraphicsManager::EnableZTest()
{
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
  mDirectXDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
}
void CGraphicsManager::DisableZTest()
{
  mDirectXDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
}
void CGraphicsManager::EnableZWrite()
{
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}
void CGraphicsManager::DisableZWrite()
{
  mDirectXDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
}
void CGraphicsManager::Present()
{
  mDirectXDevice->Present( NULL, NULL, NULL, NULL );
}
void CGraphicsManager::DrawTeapot()
{
  CEffectTechnique* EffectTechnique =
    EffectManagerInstance->GetResource( "GenerateGBufferDebugTechnique" );
  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
  UINT l_NumPasses = 0;
  l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );

  if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
    return;

  for ( UINT b = 0; b < l_NumPasses; ++b )
  {
    l_Effect->BeginPass( b );
    m_TeapotMesh->DrawSubset( 0 );
    l_Effect->EndPass();
  }

  l_Effect->End();
}
void CGraphicsManager::DrawColoredQuad2DTexturedInPixels( RECT Rect, Math::CColor& Color,
    CTexture* Texture, float U0, float V0, float U1, float V1 )
{
  SCREEN_COLOR_VERTEX v[4] =
  {
    { ( ( float )Rect.left ) - 0.5f, ( ( float )Rect.top ) - 0.5f, 0, 1, Color.GetUint32Argb(), U0, V0 }
    , { ( ( float )Rect.right ) + 0.5f, ( ( float )Rect.top ) - 0.5f, 0, 1, Color.GetUint32Argb(), U1, V0 }
    , { ( ( float )Rect.left ) - 0.5f, ( ( float )Rect.bottom ) + 0.5f, 0, 1, Color.GetUint32Argb(), U0, V1 }
    , { ( ( float )Rect.right ) + 0.5f, ( ( float )Rect.bottom ) + 0.5f, 0, 1, Color.GetUint32Argb(), U1, V1 }
  };
  mDirectXDevice->SetFVF( SCREEN_COLOR_VERTEX::GetFVF() );

  if ( Texture != NULL )
    Texture->Activate( 0 );

  mDirectXDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( SCREEN_COLOR_VERTEX ) );
}
void CGraphicsManager::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(
  CEffectTechnique* EffectTechnique, RECT Rect, Math::CColor Color, CTexture* Texture, float U0,
  float V0, float U1, float V1 )
{
  if ( EffectTechnique == NULL )
    return;

  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();

  if ( l_Effect != NULL )
  {
    mDirectXDevice->SetVertexDeclaration( SCREEN_COLOR_VERTEX::GetVertexDeclaration() );
    l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
    UINT l_NumPasses;
    l_Effect->Begin( &l_NumPasses, 0 );

    for ( UINT iPass = 0; iPass < l_NumPasses; iPass++ )
    {
      l_Effect->BeginPass( iPass );
      DrawColoredQuad2DTexturedInPixels( Rect, Color, Texture, U0, V0, U1, V1 );
      l_Effect->EndPass();
    }

    l_Effect->End();
  }
}
void CGraphicsManager::SetSrcBlend( const std::string& BlendState )
{
  mDirectXDevice->SetRenderState( D3DRS_SRCBLEND, ToD3DBlendEnum( BlendState ) );
}
void CGraphicsManager::SetDstBlend( const std::string& BlendState )
{
  mDirectXDevice->SetRenderState( D3DRS_DESTBLEND, ToD3DBlendEnum( BlendState ) );
}

D3DBLEND CGraphicsManager::ToD3DBlendEnum( const std::string& BlendState )
{
  if ( BlendState == "One" )
    return D3DBLEND_ONE;
  else if ( BlendState == "SrcColor" )
    return D3DBLEND_SRCCOLOR;
  else if ( BlendState == "InvSrcColor" )
    return D3DBLEND_INVSRCCOLOR;
  else if ( BlendState == "SrcAlpha" )
    return D3DBLEND_SRCALPHA;
  else if ( BlendState == "InvSrcAlpha" )
    return D3DBLEND_INVSRCALPHA;
  else if ( BlendState == "DestAlpha" )
    return D3DBLEND_DESTALPHA;
  else if ( BlendState == "InvDestAlpha" )
    return D3DBLEND_INVDESTALPHA;
  else if ( BlendState == "DestColor" )
    return D3DBLEND_DESTCOLOR;
  else if ( BlendState == "InvDestColor" )
    return D3DBLEND_INVDESTCOLOR;
  else if ( BlendState == "SrcAlphaSat" )
    return D3DBLEND_SRCALPHASAT;
  else if ( BlendState == "BothSrcAlpha" )
    return D3DBLEND_BOTHSRCALPHA;
  else if ( BlendState == "BothInvSrcAlpha" )
    return D3DBLEND_BOTHINVSRCALPHA;
  else if ( BlendState == "BlendFactor" )
    return D3DBLEND_BLENDFACTOR;
  else if ( BlendState == "InvBlendFactor" )
    return D3DBLEND_INVBLENDFACTOR;
  else if ( BlendState == "SrcColor2" )
    return D3DBLEND_SRCCOLOR2;
  else if ( BlendState == "InvSrcColor2" )
    return D3DBLEND_INVSRCCOLOR2;
  else if ( BlendState == "ForceDWord" )
    return D3DBLEND_FORCE_DWORD;

  return D3DBLEND_ZERO;
}

D3DBLENDOP CGraphicsManager::ToD3DBlendOPEnum( const std::string& BlendOPState )
{
  if ( BlendOPState == "Add" )
    return D3DBLENDOP_ADD;
  else if ( BlendOPState == "Subtract" )
    return D3DBLENDOP_SUBTRACT;
  else if ( BlendOPState == "RevSubtract" )
    return D3DBLENDOP_REVSUBTRACT;
  else if ( BlendOPState == "Min" )
    return D3DBLENDOP_MIN;
  else if ( BlendOPState == "Max" )
    return D3DBLENDOP_MAX;

  return D3DBLENDOP_FORCE_DWORD;
}

void CGraphicsManager::CreateQuadBuffers()
{
  Math::CColor color = Math::colBLACK;
  uint16 indices[4] = { 0, 1, 2, 3 };
  TT1_DIFF_VERTEX vertices[4] =
  {
    {  1.f, -1.f, 0.0f, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() ), 1.f, 1.f }
    , { -1.f, -1.f, 0.0f, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() ), 0.f, 1.f }
    , { -1.f, 1.f, 0.0f, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() ), 0.f, 0.f }
    , {  1.f, 1.f, 0.0f, D3DCOLOR_COLORVALUE( color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() ), 1.f, 0.f }
  };
  //Create the Vertex Buffer and Index Buffer
  void* l_Data = NULL;
  UINT l_Length = sizeof( TT1_DIFF_VERTEX ) * 4;
  mDirectXDevice->CreateVertexBuffer( l_Length, 0, TT1_DIFF_VERTEX::GetFVF(), D3DPOOL_DEFAULT,
                                      &m_VBQuad, NULL );
  m_VBQuad->Lock( 0, l_Length, &l_Data, 0 );
  memcpy( l_Data, &vertices, l_Length );
  m_VBQuad->Unlock();
  l_Length = sizeof( uint16 ) * 4;
  mDirectXDevice->CreateIndexBuffer( l_Length, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IBQuad, NULL );
  m_IBQuad->Lock( 0, l_Length, &l_Data, 0 );
  memcpy( l_Data, &indices, l_Length );
  m_IBQuad->Unlock();
}
void CGraphicsManager::DrawQuad2DTexturedInPixelsInFullScreen( CEffectTechnique* EffectTechnique )
{
  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();

  if ( l_Effect != NULL )
  {
    l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
    UINT l_NumPasses = 0;

    if ( SUCCEEDED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
    {
      mDirectXDevice->SetVertexDeclaration( SCREEN_COLOR_VERTEX::GetVertexDeclaration() );
      mDirectXDevice->SetStreamSource( 0, m_VBQuad, 0, sizeof( SCREEN_COLOR_VERTEX ) );
      mDirectXDevice->SetIndices( m_IBQuad );

      for ( UINT iPass = 0; iPass < l_NumPasses; ++iPass )
      {
        l_Effect->BeginPass( iPass );
        mDirectXDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0,
                                              static_cast<UINT>( 4 ), 0, static_cast<UINT>( 2 ) );
        l_Effect->EndPass();
      }

      l_Effect->End();
    }
  }
}

void CGraphicsManager::SetWidthAndHeight( uint32 _Width, uint32 _Height )
{
  mWidth =  _Width;
  mHeight = _Height;
}

Math::Vect2f CGraphicsManager::ToScreenCoordinates( Math::Vect3f Point )
{
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;
  D3DVIEWPORT9 pViewport;
  mDirectXDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );
  mDirectXDevice->GetTransform( D3DTS_VIEW, &viewMatrix );
  mDirectXDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
  mDirectXDevice->GetViewport( &pViewport );
  D3DXVECTOR3 l_OutPosition;
  D3DXVECTOR3 modPos( Point.u );
  D3DXVec3Project( &l_OutPosition, &modPos, &pViewport, &projectionMatrix, &viewMatrix,
                   &worldMatrix );
  // To Debug uncomment this line
  //CFontManager::GetSingletonPtr()->DrawDefaultText( l_OutPosition.x, l_OutPosition.y, Math::colWHITE,
  // "Light" );
  return Math::Vect2f( l_OutPosition.x , l_OutPosition.y );
}
Math::Vect3f CGraphicsManager::ToWorldCoordinates( Math::Vect2f Point )
{
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;
  D3DVIEWPORT9 pViewport;
  mDirectXDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );
  mDirectXDevice->GetTransform( D3DTS_VIEW, &viewMatrix );
  mDirectXDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
  mDirectXDevice->GetViewport( &pViewport );
  D3DXVECTOR3 l_OutPosition;
  D3DXVECTOR3 modPos( Point.x, Point.y, 0 );
  D3DXVec3Unproject( &l_OutPosition, &modPos, &pViewport, &projectionMatrix, &viewMatrix,
                     &worldMatrix );
  return Math::Vect3f();
}

//template <class T>
void CGraphicsManager::RenderMesh( const Math::Mat44f aTransform, LPD3DXMESH aMesh, Math::CColor aColor )
{
  SetTransform( aTransform );

  CEffectTechnique* EffectTechnique = ( aColor.GetAlpha() < 1.0f ) ?
                                      EffectManagerInstance->GetResource( "GenerateGBufferDebugTechnique" ) :
                                      EffectManagerInstance->GetResource( "GenerateGBufferDebugTechnique" );

  // Set the debug color
  EffectTechnique->SetDebugColor( aColor );
  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
  UINT l_NumPasses = 0;
  l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );

  if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
    return;

  for ( UINT b = 0; b < l_NumPasses; ++b )
  {
    l_Effect->BeginPass( b );
    aMesh->DrawSubset( 0 );
    l_Effect->EndPass();
  }

  l_Effect->End();
  SetTransform( Math::Mat44f() );
}

//void CGraphicsManager::DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, SRectangle2D& coordText)
//{
//  Vect2i finalPos = pos;
//  CalculateAlignment(w, h, alignment,finalPos);
//
//  // finalPos = [0]
//  //
//  //  [0]------[2]
//  //   |        |
//  //   |        |
//  //   |        |
//  //  [1]------[3]
//
//  Vect2f coord_text[4];
//  coord_text[0].x = coordText.m_vBottomLeft.x;
//  coord_text[0].y = coordText.m_vTopRight.y;
//  coord_text[1]       = coordText.m_vBottomLeft;
//  coord_text[2]       = coordText.m_vTopRight;
//  coord_text[3].x = coordText.m_vTopRight.x;
//  coord_text[3].y = coordText.m_vTopRight.y;
//
//  unsigned short indices[6]={0,2,1,1,2,3};
//  SCREEN_TEXTURE_VERTEX v[4] =
//  {
//          { (float)finalPos.x,        (float)finalPos.y,      0.f,1.f, coord_text[0].x,   coord_text[0].y} //(x,y) sup_esq.
//      ,   { (float)finalPos.x,        (float)finalPos.y+h,    0.f,1.f, coord_text[1].x,   coord_text[1].y} //(x,y) inf_esq.
//      , { (float)finalPos.x+w,    (float)finalPos.y,      0.f,1.f, coord_text[2].x,   coord_text[2].y} //(x,y) sup_dr.
//      ,   { (float)finalPos.x+w,  (float)finalPos.y+h,    0.f,1.f, coord_text[3].x,   coord_text[3].y} //(x,y) inf_dr.
//  };
//
//  m_pD3DDevice->SetFVF( SCREEN_TEXTURE_VERTEX::getFlags() );
//  m_pD3DDevice->SetTexture(0, texture->GetD3DXTexture() );
//  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );
//}

void CGraphicsManager::DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment,
                                   CTexture* texture, ETypeFlip flip )
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

  Math::Vect2f coord_text[4];

  //    uint32 w_aux, h_aux;
  switch ( flip )
  {
  case NONE_FLIP:
    coord_text[0].x = 0.01f;
    coord_text[0].y = 0.01f;
    coord_text[1].x = 0.01f;
    coord_text[1].y = 0.99f;
    coord_text[2].x = 0.99f;
    coord_text[2].y = 0.01f;
    coord_text[3].x = 0.99f;
    coord_text[3].y = 0.99f;
    break;

  case FLIP_X:
    //Pintamos primero el quad de la izquierda:
    coord_text[0].x = 0.99f;
    coord_text[0].y = 0.01f;
    coord_text[1].x = 0.99f;
    coord_text[1].y = 0.99f;
    coord_text[2].x = 0.01f;
    coord_text[2].y = 0.01f;
    coord_text[3].x = 0.01f;
    coord_text[3].y = 0.99f;

    /*w_aux = (uint32)(w * 0.5);
    DrawQuad2D(finalPos, w_aux, h, UPPER_LEFT, texture, NONE_FLIP);
    finalPos.x += w_aux;
    w = w - w_aux;
    coord_text[0].x = 0.99f;    coord_text[0].y = 0.f;
    coord_text[1].x = 0.99f;    coord_text[1].y = 1.f;
    coord_text[2].x = 0.f;      coord_text[2].y = 0.f;
    coord_text[3].x = 0.f;      coord_text[3].y = 1.f;*/
    break;

  case FLIP_Y:
    coord_text[0].x = 0.01f;
    coord_text[0].y = 0.99f;
    coord_text[1].x = 0.01f;
    coord_text[1].y = 0.01f;
    coord_text[2].x = 0.99f;
    coord_text[2].y = 0.99f;
    coord_text[3].x = 0.99f;
    coord_text[3].y = 0.01f;

    /*h_aux = (uint32)(h * 0.5);
    DrawQuad2D(finalPos, w, h_aux, UPPER_LEFT, texture, NONE_FLIP);
    finalPos.y += h_aux + 1;
    h = h - h_aux - 1;
    coord_text[0].x = 1.f;  coord_text[0].y = 0.f;
    coord_text[1].x = 1.f;  coord_text[1].y = 1.f;
    coord_text[2].x = 0.f;  coord_text[2].y = 0.f;
    coord_text[3].x = 0.f;  coord_text[3].y = 1.f;*/
    break;

  default:
  {
    LOG_ERROR_APPLICATION( "Manager:: Se está intentado renderizar un quad2d con un flip desconocido" );
  }

  }


  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_TEXTURE_VERTEX v[4] =
  {
    { ( float )finalPos.x, ( float )finalPos.y,      0.f, 1.f, coord_text[0].x,   coord_text[0].y}    //(x,y) sup_esq.
    ,   { ( float )finalPos.x, ( float )finalPos.y + h,    0.f, 1.f, coord_text[1].x,   coord_text[1].y}    //(x,y) inf_esq.
    , { ( float )finalPos.x + w, ( float )finalPos.y,      0.f, 1.f, coord_text[2].x,   coord_text[2].y} //(x,y) sup_dr.
    ,   { ( float )finalPos.x + w, ( float )finalPos.y + h,    0.f, 1.f, coord_text[3].x,   coord_text[3].y} //(x,y) inf_dr.
  };

  GetDevice()->SetFVF( SCREEN_TEXTURE_VERTEX::GetFVF() );
  GetDevice()->SetTexture( 0, texture->GetDXTexture() );
  GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v,
                                       sizeof( SCREEN_TEXTURE_VERTEX ) );
}

void CGraphicsManager::DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h, float yaw, ETypeAlignment alignment, CTexture* Texture )
{
  Math::Vect2i finalPos = pos;
  // Vectores Dirección
  Math::Vect2f right( cos( -yaw ), sin( -yaw ) );
  Math::Vect2f up( cos( -yaw - D3DX_PI / 2 ), sin( -yaw - D3DX_PI / 2 ) );
  // Vértices
  Math::Vect2f vertex3( finalPos.x - ( up.x * w / 2 ) + ( right.x * h / 2 ), finalPos.y - ( up.y * w / 2 ) + ( right.y * h / 2 ) );
  Math::Vect2f vertex1( finalPos.x - ( up.x * w / 2 ) - ( right.x * h / 2 ), finalPos.y - ( up.y * w / 2 ) - ( right.y * h / 2 ) );
  Math::Vect2f vertex2( finalPos.x + ( up.x * w / 2 ) + ( right.x * h / 2 ), finalPos.y + ( up.y * w / 2 ) + ( right.y * h / 2 ) );
  Math::Vect2f vertex0( finalPos.x + ( up.x * w / 2 ) - ( right.x * h / 2 ), finalPos.y + ( up.y * w / 2 ) - ( right.y * h / 2 ) );


  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short indices[6] = { 0, 2, 1, 1, 2, 3 };

  SCREEN_TEXTURE_VERTEX v[4] =
  {
    { vertex0.x, vertex0.y, 0, 1, 0.0f, 0.0f } //(x,y) sup_esq.
    , { vertex1.x, vertex1.y, 0, 1, 0.0f, 1.0f } //(x,y) inf_esq.
    , { vertex2.x, vertex2.y, 0, 1, 1.0f, 0.0f } //(x,y) sup_dr.
    , { vertex3.x, vertex3.y, 0, 1, 1.0f, 1.0f } //(x,y) inf_dr.
  };

  GetDevice()->SetFVF( SCREEN_TEXTURE_VERTEX::GetFVF() );

  GetDevice()->SetTexture( 0, Texture->GetDXTexture() );

  GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );
}

void CGraphicsManager::DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment,
                                   CTexture* texture, float U0, float V0, float U1, float V1 )
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

  Math::Vect2f coord_text[4];

  coord_text[0].x = U0;
  coord_text[0].y = V0;
  coord_text[1].x = U0;
  coord_text[1].y = V1;
  coord_text[2].x = U1;
  coord_text[2].y = V0;
  coord_text[3].x = U1;
  coord_text[3].y = V1;

  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_TEXTURE_VERTEX v[4] =
  {
    { ( float )finalPos.x, ( float )finalPos.y,      0.f, 1.f, coord_text[0].x,   coord_text[0].y}    //(x,y) sup_esq.
    ,   { ( float )finalPos.x, ( float )finalPos.y + h,    0.f, 1.f, coord_text[1].x,   coord_text[1].y}    //(x,y) inf_esq.
    , { ( float )finalPos.x + w, ( float )finalPos.y,      0.f, 1.f, coord_text[2].x,   coord_text[2].y} //(x,y) sup_dr.
    ,   { ( float )finalPos.x + w, ( float )finalPos.y + h,    0.f, 1.f, coord_text[3].x,   coord_text[3].y} //(x,y) inf_dr.
  };

  GetDevice()->SetFVF( SCREEN_TEXTURE_VERTEX::GetFVF() );
  GetDevice()->SetTexture( 0, texture->GetDXTexture() );
  GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v,
                                       sizeof( SCREEN_TEXTURE_VERTEX ) );
}

void CGraphicsManager::InitShapes()
{
  // Create the box shape
  CBoxShape::CreateGeometry();
}

void CGraphicsManager::DestroyShapes()
{
  CBoxShape::DestroyGeometry();
}