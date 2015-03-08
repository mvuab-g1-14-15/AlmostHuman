/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 g_WorldViewProjectionMatrix : WorldViewProjection;


	  float g_GaussianValue
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 1.0;
	  float UIStep = 0.01;
	  string UIName =  "g_GaussianValue";
	  > = 0.5;
	

texture T1  <
string ResourceName = "";//Optional default file name
string UIName =  "T1 Texture";
string ResourceType = "2D";
>;

sampler2D T1Sampler = sampler_state {
Texture = <T1>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Clamp;
AddressV = Clamp;
};


texture T2  <
string ResourceName = "";//Optional default file name
string UIName =  "T2 Texture";
string ResourceType = "2D";
>;

sampler2D T2Sampler = sampler_state {
Texture = <T2>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Clamp;
AddressV = Clamp;
};
	  	
	

struct VertexVS
{
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
};

struct VertexPS
{
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
	l_OUT.HPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldViewProjectionMatrix);
	l_OUT.UV=IN.UV;
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	float4 l_T1Color=tex2D(T1Sampler, IN.UV);
	
	float4 l_T2Color=tex2D(T2Sampler, IN.UV);
	return l_T1Color*g_GaussianValue+l_T2Color*(1.0-g_GaussianValue);
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