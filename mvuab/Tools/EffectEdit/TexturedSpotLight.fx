float4x4 g_WorldViewProjectionMatrix : WorldViewProjection;
float4x4 g_WorldMatrix : World < string UIWidget="None"; >;
float4x4 g_InverseViewMatrix : ViewInverse < string UIWidget="None"; >;


texture TexturedSpotLight  <
string ResourceName = "";//Optional default file name
string UIName =  "TexturedSpotLight Texture";
string ResourceType = "2D";
>;

sampler2D TexturedSpotLightSampler = sampler_state
{
	Texture = <TexturedSpotLight>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
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
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);
	float3 l_LightToPixel=normalize(IN.WorldPosition-Lamp0Point);
	float l_Diffuse=saturate(dot(Nn, -l_LightToPixel));
	float l_DotWithLightDirection=saturate(dot(l_LightToPixel, Lamp0Direction));
	float l_Angle=cos(30.0*3.1416/180.0);
	float l_Multiplier=l_DotWithLightDirection> l_Angle ? 1.0 : 0.0;
	float l_TexcoordByAngle=((l_DotWithLightDirection-l_Angle)/(1.0-l_Angle));
	float4 l_LightColor=tex2D(TexturedSpotLightSampler, float2(l_TexcoordByAngle, 0.5));
	return float4(l_Diffuse*l_LightColor.xyz*l_Multiplier, 1.0);
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
