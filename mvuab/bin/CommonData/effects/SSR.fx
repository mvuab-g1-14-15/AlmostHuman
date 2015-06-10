#include "globals.fxh"
#include "samplers.fxh"
#include "vertex_types.fxh"

// https://github.com/wallisc/ScreenSpaceReflection/blob/master/Source/DeferredRender.fx
// http://jcgt.org/published/0003/04/04/paper.pdf
// http://habrahabr.ru/post/244367/


struct VertexPS
{
    float4 HPosition        : POSITION;
    float2 UV               : TEXCOORD2;
};


VertexPS RenderVS(TNORMAL_T1_VERTEX IN)
{
    VertexPS l_OutPut =(VertexPS)0;
        
    return l_OutPut;
}

float4 RenderPS(VertexPS IN) : COLOR
{
    return float4(0.0, 0.0, 0.0, 1.0);
}

technique SSR
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderVS();
        PixelShader = compile ps_3_0 RenderPS();
    }
}