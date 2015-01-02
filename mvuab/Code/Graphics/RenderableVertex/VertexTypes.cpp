#include "RenderableVertex\VertexTypes.h"
#include "GraphicsManager.h"

LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TAN_BI_T2_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TAN_BI_T1_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TAN_BI_T2_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TAN_BI_T1_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_T2_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_T1_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_T2_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_T1_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TT2_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TT1_DIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TDIFF_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TT2_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TT1_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9 CAL3D_HW_VERTEX::s_VertexDeclaration = NULL;

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
            { 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
            { 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 72, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
            { 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
            { 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_TAN_BI_T2_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
            { 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
            { 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 68, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_TAN_BI_T1_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
            { 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
            { 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_T2_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_T1_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_T2_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TNORMAL_T1_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TT2_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_COLOR, 0 },
            { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TT1_DIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_COLOR, 0 },
            { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TDIFF_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_COLOR, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TT2_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & TT1_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 & CAL3D_HW_VERTEX::GetVertexDeclaration()
{
    if(s_VertexDeclaration==NULL)
    {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] =
        {
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
            { 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
            { 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        CGraphicsManager::GetSingletonPtr()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}