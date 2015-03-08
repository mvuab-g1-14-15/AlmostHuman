/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 g_WorldViewProj 	: WorldViewProjection;
float4x4 g_WorldMatrix 		: World < string UIWidget="None"; >;
float4x4 g_InverseViewMatrix: ViewInverse < string UIWidget="None"; >;

	
texture CubeMap
<
	string ResourceName = "";//Optional default file name
	string UIName =  "CubeMap Texture";
	string ResourceType = "CUBE";
>;

samplerCUBE CubeMapSampler = sampler_state 
{
	Texture = <CubeMap>;
	
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VertexVS
{
    float3 Position	: POSITION;
	
	float4 Normal	: NORMAL;
    float4 UV		: TEXCOORD0;
};

struct VertexPS
{
    float4 HPosition		: POSITION;
	float3 WorldNormal		: TEXCOORD0;   
};

VertexPS mainVS(VertexVS IN)
{
	VertexPS l_OUT = (VertexPS) 0;

	l_OUT.HPosition 	= mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_OUT.WorldNormal 	= mul(IN.Normal, (float3x3)g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	return texCUBE(CubeMapSampler, IN.WorldNormal);
}

technique technique0
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
S