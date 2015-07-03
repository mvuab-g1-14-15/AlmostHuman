#pragma once

#include "VertexTypes.h"

// Particle
struct TPARTICLE_VERTEX
{
    float x, y, z;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_PARTICLE;
    }
    static inline unsigned int GetFVF()
    {
        return 0;
    }
    static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
    static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
    static void ReleaseVertexDeclaration()
    {
        CHECKED_RELEASE(s_VertexDeclaration);
    }
};

struct TPARTICLE_VERTEX_INSTANCE
{
    float x, y, z;
    float size;
    float alive;
};