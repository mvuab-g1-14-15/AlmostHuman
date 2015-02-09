#include "globals.fxh"

/*float g_EnvironmentFactor
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 1.0;
float UIStep = 0.01;
string UIName =  "g_EnvironmentFactor";
> = 0.5;


texture Diffuse  <
string ResourceName = "";//Optional default file name
string UIName =  "Diffuse Texture";
string ResourceType = "2D";
>;

sampler2D DiffuseSampler = sampler_state {
Texture = <Diffuse>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Wrap;
AddressV = Wrap;
};

texture CubeMap  <
string ResourceName = "";//Optional default file name
string UIName =  "CubeMap Texture";
string ResourceType = "CUBE";
>;

samplerCUBE CubeMapSampler = sampler_state {
Texture = <CubeMap>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Wrap;
AddressV = Wrap;
};*/

sampler DiffuseTextureSampler : register( s0 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VertexVS {
    float3 Position	: POSITION;
    float3 Normal	: NORMAL;
    float4 UV		: TEXCOORD0;
};

struct VertexPS {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};

VertexPS mainVS(VertexVS IN)
{
	VertexPS l_OUT=(VertexPS)0;
	l_OUT.HPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_OUT.WorldNormal=mul(float4(IN.Normal.xyz, 1.0), (float3x3)g_WorldMatrix);
	l_OUT.UV=IN.UV;
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{	
	return g_DebugColor;
}

technique DefaultTechnique {
	pass p0 {
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
