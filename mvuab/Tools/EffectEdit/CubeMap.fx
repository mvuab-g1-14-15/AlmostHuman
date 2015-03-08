float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;
float4x4 g_InverseViewMatrix: ViewInverse;

float g_EnvironmentFactor
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
};
	

struct VertexVS {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
};

struct VertexPS {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    float3 WorldNormal	: TEXCOORD1;
    float3 WorldTangent	: TEXCOORD2;
    float3 WorldBinormal : TEXCOORD3;
	float3 WorldPosition: TEXCOORD4;
};

VertexPS mainVS(VertexVS IN)
{
	VertexPS l_OUT=(VertexPS)0;
	l_OUT.HPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_OUT.WorldNormal=mul(IN.Normal, (float3x3)g_WorldMatrix);
	l_OUT.WorldPosition=mul(float4(IN.Position, 1.0), (float3x3)g_WorldMatrix);
	l_OUT.UV=IN.UV;
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{	
	return float4(1.0, 0.0, 1.0, 1.0);
}

technique DefaultTechnique {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
