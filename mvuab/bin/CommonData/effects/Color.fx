#include "globals.fxh"

struct VERTEX_VS
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float4 Color    : COLOR;
};

struct VERTEX_PS
{
	float4 HPosition : POSITION;
	float3 Normal    : TEXCOORD0;
    float4 Color     : TEXCOORD1;
};

VERTEX_PS ColorVS(VERTEX_VS IN)
{
	VERTEX_PS l_OUT = (VERTEX_PS) 0;
    
    l_OUT.HPosition = mul(float4(IN.Position, 1.0), g_WorldViewProj);
    l_OUT.Normal = IN.Normal;
    l_OUT.Color = IN.Color;
    
    return l_OUT;
}

float4 ColorPS(VERTEX_PS IN) : COLOR
{
	return IN.Color;
}

technique ColorTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 ColorVS();
		PixelShader = compile ps_3_0 ColorPS();
	}
}