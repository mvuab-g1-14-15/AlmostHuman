#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include "d3dx9shape.h"

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "Cameras\Frustum.h"
#include <string>

class CEffectTechnique;
class CTexture;

class CGraphicsManager : public CSingleton<CGraphicsManager>
{
public:
  typedef enum { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT } ETypeAlignment;

  CGraphicsManager();
  ~CGraphicsManager();

  bool Init( HWND hWnd, bool fullscreenMode, uint32 widthScreen,
             uint32 heightScreen );
  void Update();
  void Render();
  void Release();
  void SetupMatrices();
  void BeginScene();
  void BeginRender();
  void EndScene();
  void EndRender();
  void SetTransform( D3DXMATRIX& matrix );
  void SetTransform( Math::Mat44f& matrix );
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

  // Getters and setters
  const LPDIRECT3DDEVICE9 GetDevice() const
  {
    return m_pD3DDevice;
  }

  // Basic Primitives
  void DrawAxis( float32 Size );
  void DrawIcoSphere();
  void DrawGrid( float32 Size, Math::CColor Color = Math::colWHITE,
                 int GridX = 10,
                 int32 GridZ = 10 );
  void DrawPlane( float32 Size, const Math::Vect3f& normal, float32 distance,
                  Math::CColor Color = Math::colWHITE, int GridX = 10, int32 GridZ = 10 );
  void DrawCube( float32 Size, Math::CColor Color );
  void DrawCube( float32 Size );
  void DrawBox( float32 SizeX, float32 SizeY, float32 SizeZ,
                Math::CColor Color = Math::colWHITE );
  void DrawCircle( float32 Radius, Math::CColor Color = Math::colWHITE,
                   int32 Aristas = 10 );
  void DrawSphere( float32 Radius, Math::CColor Color = Math::colWHITE,
                   int32 Aristas = 10 );
  void DrawLine( const Math::Vect3f& PosA, const Math::Vect3f& PosB,
                 Math::CColor Color = Math::colWHITE );
  void DrawQuad2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                   ETypeAlignment alignment,
                   Math::CColor color = Math::colBLUE );

  void DrawQuad2DTexturedInPixelsInFullScreen( CEffectTechnique* EffectTechnique );

  void GetRay( const Math::Vect2i& mousePos, Math::Vect3f& posRay,
               Math::Vect3f& dirRay );
  void DrawRectangle2D( const Math::Vect2i& pos, uint32 w, uint32 h,
                        Math::CColor& backGroundColor,
                        uint32 edge_w, uint32 edge_h, Math::CColor& edgeColor );
  void DrawCylinder( float32 Top_Radius, float32 Bottom_Radius, float32 h,
                     uint32 Aristas,
                     Math::CColor Color, bool drawCover );
  void DrawCapsule( float32 radius, float32 h, uint32 Aristas = 10,
                    Math::CColor Color = Math::colWHITE );
  void DrawQuad3D( const Math::Vect3f& pos, const Math::Vect3f& up,
                   const Math::Vect3f& right,
                   float32 w, float32 h, Math::CColor color = Math::colBLUE );
  void DrawQuad3D( const Math::Vect3f& ul, const Math::Vect3f& ur,
                   const Math::Vect3f& dl,
                   const Math::Vect3f& dr, Math::CColor color );
  void DrawTeapot();
  void CreateQuadBuffers();


  //TODO AÑADIDO STAGEID NO SE SI ES NECESARIO, POR DEFECTO SIEMPRE PONE 0

  void CGraphicsManager::DrawColoredQuad2DTexturedInPixels( RECT Rect, Math::CColor& Color,
      CTexture* Texture, float U0, float V0, float U1, float V1, size_t StageId );

  void DrawColoredQuad2DTexturedInPixelsByEffectTechnique(
    CEffectTechnique* EffectTechnique, RECT Rect, Math::CColor Color, CTexture* Texture,
    float U0, float V0, float U1, float V1, size_t StageId );


private: // Members
  HWND
  m_WindowId;                      // 3D render window handle
  LPDIRECT3D9             m_pD3D;                          // direct3d interface
  LPDIRECT3DDEVICE9       m_pD3DDevice;                    // direct3d device
  uint32
  m_uWidth;                        // width of the client windows
  uint32
  m_uHeight;                       // height of the client windows
  Math::CColor
  m_BackbufferColor_debug;         // Clear the backbuffer with this color in debug mode
  Math::CColor
  m_BackbufferColor_release;       // Clear the backbuffer with this color in release mode
  LPDIRECT3DSURFACE9      m_pBackBuffer;
  bool                    m_bPaintSolid;
  bool
  m_bIsOk;                         // Initialization boolean control
  LPD3DXMESH              m_SphereMesh;
  LPD3DXMESH              m_BoxMesh;
  LPD3DXMESH              m_CylinderMesh;
  LPD3DXMESH              m_TeapotMesh;

  LPDIRECT3DVERTEXBUFFER9 m_VBQuad;
  LPDIRECT3DINDEXBUFFER9  m_IBQuad;

private: // Methods
  void GetWindowRect( HWND hwnd );
  void CalculateAlignment( uint32 w, uint32 h, ETypeAlignment alignment,
                           Math::Vect2i& finalPos );
  D3DBLEND ToD3DBlendEnum( const std::string& BlendState );
};

#endif // GRAPHICS_MANAGER_H
