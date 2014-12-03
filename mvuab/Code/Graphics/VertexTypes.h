#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H
#pragma once

#include <d3dx9.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define VERTEX_TYPE_GEOMETRY 0x0001
#define VERTEX_TYPE_NORMAL 0x0002
#define VERTEX_TYPE_TANGENT 0x0004
#define VERTEX_TYPE_BINORMAL 0x0008
#define VERTEX_TYPE_TEXTURE1 0x0010
#define VERTEX_TYPE_TEXTURE2 0x0020
#define VERTEX_TYPE_DIFFUSE 0x0040

/*
G - GEOMETRY	:> FLOAT X Y Z
T - TANGENT		:> FLOAT NX, NY, NZ
N - NORMAL		:> FLOAT TNX, TNY, TNZ
B - BINORMAL	:> FLOAT BNX, BNY, BNZ
T1 - TEXTURE1	:> FLOAT TU TV
T2 - TEXTURE2	:> FLOAT TU2 TV2
D - DIFFUSE		:> UNSIGNED LONG DIFFUSE

TODOS
G T1
G N T1
G D T1
G N D T1
G T1 T2
G N T1 T
G D T1 T2
G N D T1 T2

G N T B T1
G N T B T1 T2
G N T B D T1
G N T B T1 T2
*/

struct CUSTOMVERTEX
{
	float32 x, y, z;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};

struct CUSTOMVERTEX2
{
	D3DXVECTOR3 pos;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};

struct SCREEN_COLOR_VERTEX
{
	float32 x, y, z, w;
	DWORD color;
    float32 u, v; 
	static unsigned int getFlags()
	{
		return(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	}
};

struct TCOLORED_VERTEX
{
	float32 x, y, z;
	unsigned long color;
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
	}
	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_DIFFUSE;
	}
};

struct TNORMAL_TAN_BI_T2_DIFF_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tanx, tany, tanz;
        float bx, by, bz;
        unsigned long color;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|
                                VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return 0;
        }
};

struct TNORMAL_TAN_BI_T1_DIFF_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tanx, tany, tanz;
        float bx, by, bz;
        unsigned long color;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|
                                VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return 0;
        }
};

struct TNORMAL_TAN_BI_T2_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tanx, tany, tanz;
        float bx, by, bz;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|
                                VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2;
        }
        static inline unsigned int GetFVF()
        {
                return 0;
        }
};

struct TNORMAL_TAN_BI_T1_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tanx, tany, tanz;
        float bx, by, bz;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|
                                VERTEX_TYPE_TEXTURE1;
        }
        static inline unsigned int GetFVF()
        {
                return 0;
        }
};

struct TNORMAL_T2_DIFF_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        unsigned long color;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEX2|D3DFVF_DIFFUSE;
        }
};

struct TNORMAL_T1_DIFF_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        unsigned long color;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_DIFFUSE;
        }
};

struct TNORMAL_DIFF_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        unsigned long color;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
        }
};

struct TNORMAL_T2_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEX2;
        }
};

struct TNORMAL_T1_VERTEX
{
        float x, y, z;
        float nx, ny, nz;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
        }
};

struct TT2_DIFF_VERTEX
{
        float x, y, z;
        unsigned long color;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEX2|D3DFVF_DIFFUSE;
        }
};

struct TT1_DIFF_VERTEX
{
        float x, y, z;
        unsigned long color;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE;
        }
};

struct TDIFF_VERTEX
{
        float x, y, z;
        unsigned long color;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_DIFFUSE;
        }
};

struct TT2_VERTEX
{
        float x, y, z;
        float tu, tv;
        float tu2, tv2;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEX2;
        }
};

struct TT1_VERTEX
{
        float x, y, z;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_TEX1;
        }
};

#endif // VERTEX_TYPES_H