#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include "d3dx9shape.h"

#include "Utils\Manager.h"
#include "Utils\Defines.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "Cameras\Frustum.h"
#include <string>

class CEffectTechnique;
class CTexture;
class CEngineConfig;

class CGraphicsManager : public CManager
{
public:
  typedef enum { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT } ETypeAlignment;

  CGraphicsManager();
  CGraphicsManager( CXMLTreeNode &atts );
  ~CGraphicsManager();

  void Init();
  void Update();
  void Render();
  void Release();
  void SetupMatrices();
  void BeginScene();
  void BeginRender();
  void EndScene();
  void EndRender();
  void SetTransform( D3DXMATRIX& matrix );
  void SetTransform( const Math::Mat44f& matrix );
  void EnableAlphaBlend();
  void DisableAlphaBlend();
  void EnableZBuffering();
  void DisableZBuffering();
  void GetWidthAndHeight( uint32& w, uint32& h );
  void SetWidthAndHeight( uint32 w, uint32 h );
  void RenderCursor();
  void Clear();
  void Clear( bool target, bool zbuffer, bool stencil, u_int mask );
  void EnableZTest();
  void DisableZTest();
  void EnableZWrite();
  void DisableZWrite();
  void Present();
  void EnableAlphaTest();
  void DisableAlphaTest();
  void SetBlendOP();
  void SetSrcBlend( const std::string& BlendState );
  void SetDstBlend( const std::string& BlendState );
  Math::Vect2f ToScreenCoordinates( Math::Vect3f Point );
  Math::Vect3f ToWorldCoordinates( Math::Vect2f Point );

  // Getters and setters
  const LPDIRECT3DDEVICE9 GetDevice() const
  {
    return mDirectXDevice;
  }

  // Basic Primitives
  void DrawAxis( float32 Size );

  void DrawIcoSphere();

  void DrawCube( const Math::Vect3f& aPosition, float32 Size, Math::CColor Color );
  void DrawCube( const Math::Mat44f& aTransform, float32 Size, const Math::CColor aColor = Math::colWHITE );
  void DrawBox( const Math::Vect3f& aPosition, float32 SizeX, float32 SizeY, float32 SizeZ,
                Math::CColor Color = Math::colWHITE );
  void DrawBox( const Math::Mat44f& aTransform, float32 SizeX, float32 SizeY, float32 SizeZ,
                Math::CColor Color = Math::colWHITE );
  void DrawSphere( const Math::Vect3f& aPosition, float32 Radius, Math::CColor Color = Math::colWHITE,
                   int32 Aristas = 10 );
  void DrawSphere( const Math::Mat44f& aTransform, float32 Radius, Math::CColor Color = Math::colWHITE,
                   int32 Aristas = 10 );
  void DrawCylinder( const Math::Vect3f& aPosition, float32 Top_Radius, float32 Bottom_Radius, float32 h, uint32 Aristas,
                     Math::CColor Color, bool drawCover );
  void DrawCylinder( const Math::Mat44f& aTransform, float32 Top_Radius, float32 Bottom_Radius, float32 h, uint32 Aristas,
                     Math::CColor Color, bool drawCover );
  void DrawCapsule( const Math::Vect3f& aPosition, float32 radius, float32 h, uint32 Aristas = 10,
                    Math::CColor Color = Math::colWHITE );

  void DrawGrid( float32 Size, Math::CColor Color = Math::colWHITE, int GridX = 10, int32 GridZ = 10 );
  void DrawPlane( float32 Size, const Math::Vect3f& normal, float32 distance, Math::CColor Color = Math::colWHITE,
                  int GridX = 10, int32 GridZ = 10 );
  void DrawCircle( float32 Radius, Math::CColor Color = Math::colWHITE,
                   int32 Aristas = 10 );
  void DrawLine( const Math::Vect3f& PosA, const Math::Vect3f& PosB,
                 Math::CColor Color = Math::colWHITE );
  void DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                   ETypeAlignment alignment,
                   Math::CColor color = Math::colBLUE );
  void GetRay( const Math::Vect2i& mousePos, Math::Vect3f& posRay, Math::Vect3f& dirRay );
  void DrawRectangle2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                        Math::CColor& backGroundColor,
                        uint32 edge_w, uint32 edge_h, Math::CColor& edgeColor );
  void DrawQuad2DTexturedInPixelsInFullScreen( CEffectTechnique* EffectTechnique );
  void DrawQuad3D( const Math::Vect3f& pos, const Math::Vect3f& up,
                   const Math::Vect3f& right,
                   float32 w, float32 h, Math::CColor color = Math::colBLUE );
  void DrawQuad3D( const Math::Vect3f& ul, const Math::Vect3f& ur,
                   const Math::Vect3f& dl,
                   const Math::Vect3f& dr, Math::CColor color );
  void DrawQuad3DWithTechnique( const Math::Vect3f& ul, const Math::Vect3f& ur,
                                const Math::Vect3f& dl,
                                const Math::Vect3f& dr, const Math::Vect3f& n, CEffectTechnique* EffectTechnique,
                                CTexture* Texture );
  void DrawHalfLowerSphere( const Math::Vect3f& Pos, float radius, uint32 edges, Math::CColor color );
  void DrawHalfUpperSphere( const Math::Vect3f& Pos, float radius, uint32 edges, Math::CColor color );

  void DrawTeapot();
  void CreateQuadBuffers();

  void CGraphicsManager::DrawColoredQuad2DTexturedInPixels( RECT Rect, Math::CColor& Color,
      CTexture* Texture, float U0, float V0, float U1, float V1 );

  void DrawColoredQuad2DTexturedInPixelsByEffectTechnique(
    CEffectTechnique* EffectTechnique, RECT Rect, Math::CColor Color, CTexture* Texture,
    float U0, float V0, float U1, float V1 );

private: // Members
  HWND                    m_WindowId;                      // 3D render window handle
  LPDIRECT3D9             mDirectXObject;                  // direct3d interface
  LPDIRECT3DDEVICE9       mDirectXDevice;                  // direct3d device
  uint32                  mWidth;                          // width of the client windows
  uint32                  mHeight;                         // height of the client windows
  Math::CColor            m_BackbufferColor_debug;         // Clear the backbuffer with this color in debug mode
  Math::CColor            m_BackbufferColor_release;       // Clear the backbuffer with this color in release mode
  LPDIRECT3DSURFACE9      m_pBackBuffer;
  bool                    m_bPaintSolid;
  bool                    mOk;                             // Initialization boolean control
  LPD3DXMESH              m_SphereMesh;
  LPD3DXMESH              m_BoxMesh;
  LPD3DXMESH              m_CylinderMesh;
  LPD3DXMESH              m_TeapotMesh;
  LPDIRECT3DVERTEXBUFFER9 m_VBQuad;
  LPDIRECT3DINDEXBUFFER9  m_IBQuad;
  Math::Vect2i            m_ScreenResolution;
  Math::Vect2i            m_ScreenSize;
  Math::Vect2i            m_ScreenPosition;
  bool                    m_FullScreenMode;
  bool                    m_FitDesktop;
  bool                    m_Windowed;
  int                     m_RefreshRate;

private: // Methods
  void CalculateAlignment( uint32 w, uint32 h, ETypeAlignment alignment,
                           Math::Vect2i& finalPos );
  D3DBLEND ToD3DBlendEnum( const std::string& BlendState );

  DWORD GetBehaviorFlags();
  bool CreateFullScreenMode();
  bool CreateWindowedMode();
  void RenderMesh( const Math::Mat44f aTransform, LPD3DXMESH aMesh , Math::CColor aColor );
};

#endif // GRAPHICS_MANAGER_H
