//Ommi FX
/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/


	  float g_Intensity
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 3.0;
	  float UIStep = 0.01;
	  string UIName =  "g_Intensity";
	  > = 1.0;
	

	  float g_SpecularExponent
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 200.0;
	  float UIStep = 1.0;
	  string UIName =  "g_SpecularExponent";
	  > = 20;
	

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
	
	  float g_NearAt
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 50.0;
	  float UIStep = 0.01;
	  string UIName =  "g_NearAt";
	  > = 2;
	  
	  
	  float g_FarAt
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 50.0;
	  float UIStep = 0.01;
	  string UIName =  "g_FarAt";
	  > = 5;
	  float3 Lamp0Point : POSITION
	  <
	  string Object = "Point Light 0";
	  string UIName =  "Lamp 0 Position";
	  string Space = "World";
	  > = {-0.5f,2.0f,1.25f};

	  float3 Lamp0Color : COLOR <
	  string Object = "Point Light 0";
	  string UIName =  "Lamp 0 Color";
	  string UIWidget = "Color";
	  > = {1.0f,1.0f,1.0f};
	  
	  float3 Lamp0Dir : DIRECTION
	  <
	  string Object = "Point Light 0";
	  string UIName =  "Lamp 0 Direction";
	  string Space = "World";
	  > = {0.0f, 0.0f, 0.0f};
	  
	  float Lamp0Angle = 20.0f;
	  float Lamp0AngleOff = 30.0f;
struct VertexVS 
{
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
};

/* data passed from vertex shader to pixel shader */
struct VertexPS 
{
    float4 HPosition		: POSITION;
    float2 UV				: TEXCOORD0;
	float3 Normal			: TEXCOORD1;
    float3 LightVec			: TEXCOORD2;
	float3 WorldPosition	: TEXCOORD3;
    float3 WorldNormal		: TEXCOORD4;
    float3 WorldView		: TEXCOORD5;
};

float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix : WORLD;
float4x4 g_ViewMatrix : WorldViewInverse;

VertexPS mainVS(VertexVS IN)
{
	VertexPS OUT = (VertexPS)0;
	OUT.HPosition = mul(float4(IN.Position.xyz, 1.0), g_WorldViewProjectionMatrix);
    OUT.UV=IN.UV;
    OUT.Normal=mul(IN.Normal,g_WorldMatrix);
    OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix).xyz;
    return OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	float l_DistanceLightPixel = length(IN.WorldPosition - Lamp0Point);
	float l_Attenuation= 1 - saturate((l_DistanceLightPixel - g_NearAt)/(g_FarAt - g_NearAt));
	float3 l_Nn=normalize(IN.Normal);
	float4 l_MaterialColor=tex2D(DiffuseSampler, IN.UV);
	float3 l_OmniLightToPixelDir = normalize(IN.WorldPosition - Lamp0Point);
	float3 l_DiffuseContrib=saturate(dot(l_Nn, -l_OmniLightToPixelDir))*l_MaterialColor.xyz*Lamp0Color*g_Intensity*l_Attenuation;
	float3 l_CameraPostion=g_ViewMatrix[3].xyz;
	float3 l_Hn=normalize(normalize(l_CameraPostion-IN.WorldPosition)-l_OmniLightToPixelDir);
    float3 l_SpecularContrib=Lamp0Color*pow(saturate(dot(l_Nn, l_Hn)), g_SpecularExponent)*g_Intensity*l_Attenuation;
	
	//1 - saturate((l_DistanceLightPixel - g_NearAt)/(g_FarAt - g_NearAt));
	float cosLamp0Angle = cos((Lamp0Angle * 3.14151692 / 180.0) / 2);
	float cosLamp0AngleOff = cos((Lamp0AngleOff * 3.14151692 / 180.0) / 2);
	float l_SpotLight = 1 - saturate((dot(l_OmniLightToPixelDir, Lamp0Dir) - cosLamp0Angle)/(cosLamp0AngleOff - cosLamp0Angle));

	return l_SpotLight*float4(l_DiffuseContrib + l_SpecularContrib, 1.0);
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
