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
    float3 Position  : TEXCOORD0;
	float3 Normal    : TEXCOORD1;
    float4 Color     : TEXCOORD2;
};

VERTEX_PS ColorVS(VERTEX_VS IN)
{
	VERTEX_PS l_OUT = (VERTEX_PS) 0;
    
    l_OUT.HPosition = mul(float4(IN.Position, 1.0), g_WorldViewProj);
    l_OUT.Position = mul(float4(IN.Position, 1.0), g_WorldMatrix).xyz;
    
    l_OUT.Normal = IN.Normal;
    l_OUT.Color = IN.Color;
    
    return l_OUT;
}

float4 ColorPS(VERTEX_PS IN) : COLOR
{
    float3 l_LightDirection = normalize(IN.Position - g_LightsPosition[0]);
    float3 l_Normal = normalize(IN.Normal);
    
    float t = dot(l_Normal, -l_LightDirection);
	return IN.Color * t;
}

technique ColorTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 ColorVS();
		PixelShader = compile ps_3_0 ColorPS();
	}
}