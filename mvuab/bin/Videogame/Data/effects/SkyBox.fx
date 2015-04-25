#include "globals.fxh"

samplerCUBE cubeSampler : register (s0) = sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float3 TextureCoordinate : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput IN)
{
	VertexShaderOutput OUT = (VertexShaderOutput) 0;
	
    OUT.Position = mul(float4(IN.Position, 1.0), g_WorldViewProj).xyww;
	OUT.TextureCoordinate = IN.Position;
	
	return OUT;
}

float4 PixelShaderFunction(VertexShaderOutput IN) : COLOR0
{
	return texCUBE(cubeSampler, IN.TextureCoordinate);
}

technique SkyBox
{
	pass p0
	{
		CullMode = None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}
