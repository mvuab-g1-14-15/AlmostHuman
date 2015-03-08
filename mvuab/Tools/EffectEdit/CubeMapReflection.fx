/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 g_WorldViewProj 	: WorldViewProjection;
float4x4 g_WorldMatrix 		: World < string UIWidget="None"; >;
float4x4 g_InverseViewMatrix: ViewInverse < string UIWidget="None"; >;

float g_EnvironmentFactor
<
	string UIWidget = "slider";
	string UIName =  "g_EnvironmentFactor";
	
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.5;
	

texture Diffuse
<
	string ResourceName = "";//Optional default file name
	string UIName =  "Diffuse Texture";
	string ResourceType = "2D";
>;

sampler2D DiffuseSampler = sampler_state 
{
	Texture = <Diffuse>;
	
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	
	AddressU = Wrap;
	AddressV = Wrap;
};
	
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
	
	AddressU = Wrap;
	AddressV = Wrap;
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
    float4 HPosition		: POSITION;
    float2 UV				: TEXCOORD0;
    float3 WorldNormal		: TEXCOORD1;
    float3 WorldTangent		: TEXCOORD2;
    float3 WorldBinormal 	: TEXCOORD3;
	float3 WorldPosition	: TEXCOORD4;
};

VertexPS mainVS(VertexVS IN)
{
	VertexPS l_OUT = (VertexPS) 0;
	
	l_OUT.UV 			= IN.UV;
	l_OUT.HPosition 	= mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	
	l_OUT.WorldNormal	= mul(IN.Normal, (float3x3)g_WorldMatrix);
	l_OUT.WorldPosition	= mul(IN.Position, (float3x3) g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	float3 l_CameraPosition = g_InverseViewMatrix[3].xyz;
	float3 l_CameraToPixel = normalize(l_CameraPosition - IN.WorldPosition);
	
	float3 Nn =	normalize(IN.WorldNormal);	
	float3 l_ReflectVector = reflect(l_CameraToPixel, Nn);
	
	float4 l_DiffuseColor = tex2D(DiffuseSampler, IN.UV);
	float4 l_EnvironmentColor = texCUBE(CubeMapSampler, l_ReflectVector);
	
	return l_DiffuseColor + l_EnvironmentColor * g_EnvironmentFactor;
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
