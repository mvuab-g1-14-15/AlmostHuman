float4x4 g_WorldViewProjectionMatrix : WorldViewProjection;



	  float g_CurrentTime
	  <
	  string UIWidget = "slider";
	  float UIMin = 0.0;
	  float UIMax = 50.0;
	  float UIStep = 0.1;
	  string UIName =  "g_CurrentTime";
	  > = 0.0;
	

texture Diffuse  <
string ResourceName = "";//Optional default file name
string UIName =  "Diffuse Texture";
string ResourceType = "2D";
>;

float3 g_WindVector=float3(1,0,0);

sampler2D DiffuseSampler = sampler_state {
Texture = <Diffuse>;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Linear;
AddressU = Wrap;
AddressV = Clamp;
};

float g_AngularSpeed=20.0/2.0;


float g_OffsetDisplacement
<
string UIWidget = "slider";
float UIMin = 0.0;
float UIMax = 0.20;
float UIStep = 0.01;
string UIName =  "g_OffsetDisplacement";
> = 0.08;
	

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
	
	l_OUT.UV=float2(IN.UV.x, 1.0-IN.UV.y);
	
	float l_Angle=g_CurrentTime*g_AngularSpeed*3.1416/180.0;
	float l_Offset=g_OffsetDisplacement*sin(l_Angle)*IN.UV.y;
	l_OUT.HPosition=mul(float4(IN.Position.xyz+l_Offset*g_WindVector, 1.0), g_WorldViewProjectionMatrix);	
	
	return l_OUT;
}

float4 mainPS(VertexPS IN) : COLOR
{
	return tex2D(DiffuseSampler, IN.UV);
}

technique technique0 {
	pass p0 {
		AlphaTestEnable = true;
		AlphaRef = 255;
		AlphaFunc = Equal;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
