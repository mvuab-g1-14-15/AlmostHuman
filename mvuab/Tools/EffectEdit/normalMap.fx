float4x4 g_WorldViewProjectionMatrix : WorldViewProjection;
float4x4 g_WorldMatrix : World < string UIWidget="None"; >;
float4x4 g_InverseViewMatrix : ViewInverse < string UIWidget="None"; >;


float g_SpecularExponent
<
string UIWidget = "slider";
float UIMin = 10.0;
float UIMax = 200.0;
float UIStep = 0.01;
string UIName =  "g_SpecularExponent";
> = 10;
	
float g_Bump
<
string UIWidget = "slider";
float UIMin = 2.0;
float UIMax = 20.0;
float UIStep = 0.5;
string UIName =  "g_Bump";
> = 2.0;
	

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


texture Normal  <
string ResourceName = "";//Optional default file name
string UIName =  "Normal Texture";
string ResourceType = "2D";
>;

sampler2D NormalSampler = sampler_state {
Texture = <Normal>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Wrap;
AddressV = Wrap;
};

float3 Lamp0Direction : DIRECTION
<
string Object = "Directional Light 0";
string UIName =  "Lamp 0 Direction";
string Space = "World";
> = {-0.5f,2.0f,1.25f};

float3 Lamp_0_color : COLOR <
string Object = "Directional Light 0";
string UIName =  "Lamp 0 Color";
string UIWidget = "Color";
> = {1.0f,1.0f,1.0f};


float3 Lamp0Point : POSITION
<
string Object = "Directional Light 0";
string UIName =  "Lamp 0 Position";
string Space = "World";
> = {-0.5f,2.0f,1.25f};
	

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
	l_OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix);
	l_OUT.WorldNormal=mul(IN.Normal.xyz, (float3x3)g_WorldMatrix);
	l_OUT.WorldTangent=mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix);
	l_OUT.WorldBinormal=mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix);
	l_OUT.UV=IN.UV;
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	float3 bump=g_Bump*(tex2D(NormalSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	float4 l_DiffuseColor=tex2D(DiffuseSampler, IN.UV);
	//return l_DiffuseColor;
	float4 l_NormalMapColor=tex2D(NormalSampler, IN.UV);
	float l_DiffuseContrib=saturate(dot(-Lamp0Direction, Nn));
	float3 l_CameraPosition=g_InverseViewMatrix[3].xyz;
	float3 Hn=normalize(normalize(l_CameraPosition-IN.WorldPosition)-Lamp0Direction);
	float l_SpecularContrib=pow(saturate(dot(Hn,Nn)), g_SpecularExponent);
	//return float4(l_SpecularContrib,l_SpecularContrib,l_SpecularContrib,1.0);
	return float4(l_DiffuseContrib*l_DiffuseColor.xyz+l_SpecularContrib*Lamp_0_color, 1.0);
	/*float3 Nn=normalize(IN.WorldNormal);
	float3 l_LightToPixel=normalize(IN.WorldPosition-Lamp0Point);
	float l_Diffuse=saturate(dot(Nn, -l_LightToPixel));
	float l_DotWithLightDirection=saturate(dot(l_LightToPixel, Lamp0Direction));
	float l_Angle=cos(30.0*3.1416/180.0);
	float l_Multiplier=l_DotWithLightDirection> l_Angle ? 1.0 : 0.0;
	float l_TexcoordByAngle=((l_DotWithLightDirection-l_Angle)/(1.0-l_Angle));*/
	//float4 l_LightColor=tex2D(TexturedSpotLightSampler, float2(l_TexcoordByAngle, 0.5));
	//return float4(l_Diffuse*l_LightColor.xyz*l_Multiplier, 1.0);
	return float4(1,0,0,1);
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