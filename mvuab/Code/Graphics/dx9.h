#pragma once
#ifndef D3DX9_H
#define D3DX9_H

#include <d3dx9.h>
#include <d3dx9shape.h>
#include <d3dx9effect.h>
#include <d3dx9math.h>

#if defined(DEBUG) | defined(_DEBUG)

#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif

#ifndef D3D_ERROR_CASE
#define D3D_ERROR_CASE( param ) case param: ASSERT( SUCCEEDED(lHR), "%s", ""#param"" ); break;
#endif

#ifndef HR
#define HR(x)                                                  \
    {                                                          \
      HRESULT lHR = x;                                         \
      if(FAILED(lHR))                                          \
      {                                                        \
         switch ( lHR )                                        \
         {                                                     \
            D3D_ERROR_CASE( D3DERR_NOTAVAILABLE )              \
            D3D_ERROR_CASE( D3DERR_INVALIDCALL )               \
            D3D_ERROR_CASE( D3DERR_NOTFOUND )                  \
            D3D_ERROR_CASE( D3DERR_DRIVERINVALIDCALL )         \
            D3D_ERROR_CASE( E_OUTOFMEMORY )                    \
            D3D_ERROR_CASE( D3DERR_OUTOFVIDEOMEMORY )          \
            D3D_ERROR_CASE( D3DERR_CANNOTPROTECTCONTENT )      \
            D3D_ERROR_CASE( D3DERR_CONFLICTINGRENDERSTATE )    \
            D3D_ERROR_CASE( D3DERR_CONFLICTINGTEXTUREFILTER )  \
            D3D_ERROR_CASE( D3DERR_CONFLICTINGTEXTUREPALETTE ) \
            D3D_ERROR_CASE( D3DERR_DEVICEHUNG )                \
            D3D_ERROR_CASE( D3DERR_DEVICELOST )                \
            D3D_ERROR_CASE( D3DERR_DEVICENOTRESET )            \
            D3D_ERROR_CASE( D3DERR_DEVICEREMOVED )             \
            D3D_ERROR_CASE( D3DERR_DRIVERINTERNALERROR )       \
            D3D_ERROR_CASE( D3DERR_INVALIDDEVICE )             \
            D3D_ERROR_CASE( D3DERR_MOREDATA )                  \
            D3D_ERROR_CASE( D3DERR_TOOMANYOPERATIONS )         \
            D3D_ERROR_CASE( D3DERR_WRONGTEXTUREFORMAT )        \
            D3D_ERROR_CASE( D3DERR_PRESENT_STATISTICS_DISJOINT )  \
            D3D_ERROR_CASE( D3DERR_WASSTILLDRAWING )           \
            D3D_ERROR_CASE( D3DERR_UNSUPPORTEDFACTORVALUE )    \
            D3D_ERROR_CASE( D3DERR_UNSUPPORTEDOVERLAY )        \
            D3D_ERROR_CASE( D3DERR_UNSUPPORTEDCRYPTO )         \
            D3D_ERROR_CASE( D3DERR_UNSUPPORTEDCOLOROPERATION)  \
            D3D_ERROR_CASE( D3DERR_UNSUPPORTEDCOLORARG)        \
         default:                                              \
            ASSERT(false, "Error in %s", ""#x"" );             \
         }                                                     \
      }                                                        \
    }
#endif
#else
#ifndef HR
#define HR(x) {HRESULT lHR = x; lHR;}
#endif
#endif

#endif// D3DX9_H