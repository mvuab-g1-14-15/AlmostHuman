float g_SpecularExponent
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 200.0;
float UIStep = 1.0;
string UIName =  "g_SpecularExponent";
> = 20;

float g_LightIntensity
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 3.0;
float UIStep = 0.01;
string UIName =  "g_LightIntensity";
> = 0.5;

float g_NearAt
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 50.0;
float UIStep = 0.01;
string UIName =  "g_NearAt";
> = 20;


float g_FarAt
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 50.0;
float UIStep = 0.01;
string UIName =  "g_FarAt";
> = 50;

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

float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_World : World;
float4x4 g_ViewInverseMatrix : ViewInverse;

struct VertexVS
{
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
};

struct VertexPS
{
    float4 HPosition		: POSITION;
    float2 UV				: TEXCOORD0;
	float3 Normal			: TEXCOORD1;
	float3 WorldPosition	: TEXCOORD2;
    float3 WorldNormal		: TEXCOORD3;
    float3 WorldView		: TEXCOORD4;
};

float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix : WORLD;

VertexPS mainVS(VertexVS IN)
{
	VertexPS l_OUT=(VertexPS)0;
	l_OUT.HPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_OUT.WorldNormal=mul(IN.Normal, (float3x3)g_World);
	l_OUT.UV=IN.UV;
	l_OUT.WorldPosition=mul(float4(IN.Position, 1.0), g_World);
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR 
{
	float4 l_MaterialColor=tex2D(DiffuseSampler, IN.UV);
	float l_DistanceLightPixel = length(IN.WorldPosition - Lamp0Point);
	float l_DistanceAttenuation= 1 - saturate((l_DistanceLightPixel - g_NearAt)/(g_FarAt - g_NearAt));
	
	float3 l_Nn=normalize(IN.WorldNormal);
	float3 l_PixelToLightPosition=normalize(Lamp0Point-IN.WorldPosition);
	float l_Angle=cos((60.0*3.1416/180.0)/2.0);
	float l_AngleFallOff=cos((80.0*3.1416/180.0)/2.0);
	float l_SpotDotValue=dot(-l_PixelToLightPosition, Lamp0Direction);
	float l_AngleAttenuation=saturate((l_SpotDotValue-l_AngleFallOff)/(l_Angle-l_AngleFallOff));
	float3 l_DiffuseContrib=saturate(dot(l_PixelToLightPosition, l_Nn));
	
	float3 l_CameraPostion=g_ViewInverseMatrix[3].xyz;
	float3 l_Hn=normalize(normalize(l_CameraPostion-IN.WorldPosition)-Lamp0Direction);
    float3 l_SpecularContrib=pow(saturate(dot(l_Nn, l_Hn)), g_SpecularExponent);
	
	float3 l_PixelColor = (l_DiffuseContrib + l_SpecularContrib) 
						* g_LightIntensity 
						* l_DistanceAttenuation
						* Lamp_0_color
						* l_AngleAttenuation
						* l_MaterialColor.xyz;
						
	return float4(l_PixelColor,1.0);
	return float4(Lamp0Point,1.0);
	return float4(1.0, 0.0, 0.0, 1.0);
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