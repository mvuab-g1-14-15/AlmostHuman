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
	float x, y, z, w;
	DWORD color;
    float u, v; 
	static unsigned int getFlags()
	{
		return(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	}
};

struct TCOLORED_VERTEX
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


#endif // VERTEX_TYPES_H