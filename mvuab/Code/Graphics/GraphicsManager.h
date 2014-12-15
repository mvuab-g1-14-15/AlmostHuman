#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#pragma once

#include <Windows.h>
#include <d3dx9.h>

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "Frustum.h"

class CCamera;

class CGraphicsManager : public CSingleton<CGraphicsManager>
{
public:
    typedef enum{ CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT } ETypeAlignment;

    CGraphicsManager();
    ~CGraphicsManager();

    bool Init (HWND hWnd, bool fullscreenMode, uint32 widthScreen, uint32 heightScreen);
    void Update(float32 deltaTime);
    void Render();
    void Release();
    void SetupMatrices( CCamera* ap_Camera );
    void BeginRender();
    void EndRender();
    void SetTransform (D3DXMATRIX& matrix);
    void SetTransform (Mat44f& matrix);
    void EnableAlphaBlend();
    void DisableAlphaBlend();
    void EnableZBuffering();
    void DisableZBuffering();
    void GetWidthAndHeight( uint32& w,uint32& h);
    void RenderCursor();

    uint32 GetNumberOfVisibleObjects()
    {
        return m_VisibleObjects;
    }

    // Getters and setters
    const LPDIRECT3DDEVICE9 GetDevice() const
    { 
        return m_pD3DDevice;
    }

    // GetSet
    GET_SET(uint32, uWidth);
    GET_SET(uint32, uHeight);

    CCamera* GetCurrentCamera(){ return m_pCurrentCamera;}

    // Basic Primitives
    void DrawAxis(float32 Size);
    void DrawIcoSphere();
    void DrawGrid(float32 Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );
    void DrawPlane(    float32 Size, const Vect3f& normal, float32 distance,CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );
    void DrawCube(float32 Size, CColor Color=colWHITE);
    void DrawBox(float32 SizeX, float32 SizeY, float32 SizeZ, CColor Color=colWHITE);
    void DrawCircle(float32 Radius, CColor Color=colWHITE, int32 Aristas=10);
    void DrawSphere(float32 Radius, CColor Color=colWHITE, int32 Aristas=10);
    void DrawLine(const Vect3f &PosA, const Vect3f &PosB, CColor Color=colWHITE);
    void DrawCamera (CCamera* camera);
    void DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);
    void GetRay (const Vect2i& mousePos, Vect3f& posRay, Vect3f& dirRay);
    void DrawRectangle2D ( const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor );
    void DrawCylinder(float32 Top_Radius, float32 Bottom_Radius, float32 h, uint32 Aristas, CColor Color, bool drawCover);
    void DrawCapsule (    float32 radius, float32 h, uint32 Aristas=10, CColor Color=colWHITE );
    void DrawQuad3D(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float32 w, float32 h, CColor color = colBLUE);
    void DrawQuad3D(    const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CColor color);


private:
    HWND                m_WindowId;                        // 3D render window handle
    LPDIRECT3D9            m_pD3D;                            // direct3d interface
    LPDIRECT3DDEVICE9   m_pD3DDevice;                    // direct3d device
    uint32                m_uWidth;                        // width of the client windows
    uint32                m_uHeight;                        // height of the client windows
    CColor                m_BackbufferColor_debug;        // Clear the backbuffer with this color in debug mode 
    CColor                m_BackbufferColor_release;        // Clear the backbuffer with this color in release mode
    LPDIRECT3DSURFACE9    m_pBackBuffer;
    CFrustum            m_Frustum;
    bool                m_bPaintSolid;
    bool                m_bIsOk;                        // Initialization boolean control
    void GetWindowRect( HWND hwnd );
    uint32                m_VisibleObjects;

    CCamera*            m_pCurrentCamera;

    void CGraphicsManager::CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);
};

#endif // GRAPHICS_MANAGER_H
