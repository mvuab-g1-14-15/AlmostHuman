#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

struct TMultiRenderTargetPixel
{
	float4 Albedo 	: COLOR0; //Albedo (float3) + (float) SpecularFactor
	float4 Ambient  : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 Normal   : COLOR2; //Normal (float3) + (float) Not used
	float4 Depth    : COLOR3; //Depth (float4)
};

struct VertexPS
{
	float4 HPosition 	: POSITION;
	float2 Pos 			: TEXCOORD0;
    float3 Normal 		: TEXCOORD1;
	float2 UV 			: TEXCOORD2;
};

VertexPS mainVS(TNORMAL_T1_VERTEX IN)
{
	VertexPS OUT=(VertexPS)0;
	OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
	OUT.Pos.xy=OUT.HPosition.zw;
	OUT.Normal = mul(IN.Normal, (float3x3)g_WorldMatrix);
	OUT.UV=IN.UV;
	return OUT;
}

TMultiRenderTargetPixel mainPS(VertexPS IN) : COLOR
{
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	
	float3 l_Diffuse = (float3)tex2D(S0LinearSampler, IN.UV);
	float4 l_Albedo = float4( l_Diffuse, 1.0f);
	float4 l_Ambient = float4( (float3)g_AmbientLight*l_Diffuse, 1.0f);
	float4 l_Normal = float4(Normal2Texture(normalize(IN.Normal)), 0.0f);
	float4 l_Depth = IN.Pos.x/IN.Pos.y;
	
	OUT.Albedo = l_Albedo;
	OUT.Ambient = l_Ambient;
	OUT.Normal = l_Normal;
	OUT.Depth = l_Depth;
	
	return OUT;
}

technique GenerateGBufferTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
