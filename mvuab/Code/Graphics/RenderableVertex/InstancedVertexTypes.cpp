#include "RenderableVertex\InstancedVertexTypes.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

LPDIRECT3DVERTEXDECLARATION9 TPARTICLE_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 &TPARTICLE_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            {0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
            {1, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
            {1, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
            D3DDECL_END()
        };
        GraphicsInstance->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}