#include "globals.fxh"

sampler DiffuseSampler : register ( s0 ) = sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

#ifdef _NORMAL_MAP
sampler NormalSampler : register ( s1 ) = sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
#endif

#ifdef _SELF_ILLUM
sampler LightMapSampler : register(
#ifdef _NORMAL_MAP
s2
#else
s1
#endif
) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};
#endif

#ifdef _REFLECTION
samplerCUBE ReflectionSampler : register (
#ifdef _SELF_ILLUM
#ifdef _NORMAL_MAP
s3
#else
s2
#endif
#else
#ifdef _NORMAL_MAP
s2
#else
s1
#endif
) = sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
#endif

struct VertexVS
{
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
#ifdef _NORMAL_MAP
    float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
#endif
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
#ifdef _SELF_ILLUM
    float2 	UV2 		: TEXCOORD1;
#endif
};

struct VertexPS
{
    float4 HPosition		: POSITION;
    float2 UV 				: TEXCOORD0;
#ifdef _SELF_ILLUM
	float2 UV2				: TEXCOORD1;
#endif
    float3 Normal 			: TEXCOORD2;
	float3 WorldPosition 	: TEXCOORD3;
#ifdef _NORMAL_MAP
	float3 WorldTangent		: TEXCOORD4;
    float3 WorldBinormal 	: TEXCOORD5;
#endif
};

VertexPS RenderVS(VertexVS IN)
{
    VertexPS OUT=(VertexPS)0;
    OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.UV=IN.UV;
#ifdef _SELF_ILLUM
	OUT.UV2=IN.UV2;
#endif
    OUT.Normal=mul(IN.Normal, (float3x3)g_WorldMatrix);
    OUT.WorldPosition=mul(float4(IN.Position,1.0), g_WorldMatrix);
#ifdef _NORMAL_MAP
	OUT.WorldTangent=mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix);
	OUT.WorldBinormal=mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix);
#endif
    return OUT;
}

float4 RenderPS(VertexPS IN) : COLOR
{
	// Obtain the normal of the face and the position
	float3 l_Normal= normalize(IN.Normal);
	
#ifdef _NORMAL_MAP
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);

	float3 bump=g_Bump*(tex2D(NormalSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	l_Normal = l_Normal + bump.x*Tn + bump.y*Bn;
	l_Normal = normalize(l_Normal);
#endif
			
	float3 l_Position = IN.WorldPosition;

	float4 l_DiffuseColor = tex2D(DiffuseSampler, IN.UV);
	
	float4 l_LightsContrib = float4(0.0, 0.0, 0.0, 1.0);
	
	for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i] == 1) 
        {
			float3 l_LightDirection = normalize(l_Position-g_LightsPosition[i]);
			float l_Attenuation = DistanceAttenuation(i, l_LightDirection );
            if(OMNI_LIGHT == g_LightsType[i])
            {
            }
            if(DIR_LIGHT == g_LightsType[i])
            {
				l_LightDirection = normalize(g_LightsDirection[i]);
            }
            else if(SPOT_LIGHT == g_LightsType[i])
            {	
				// Modify the distance attenuation with the spot attenuation
				l_Attenuation = l_Attenuation * SpotAttenuation(i, l_LightDirection);
            }

			float3 l_Hn=normalize(normalize(g_CameraPosition-l_Position)-l_LightDirection);
			float3 l_DiffuseContrib = saturate(dot(l_Normal,-l_LightDirection)) * l_Attenuation * g_LightsColor[i];
			float l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * l_Attenuation * g_LightsColor[i];
			l_LightsContrib = l_LightsContrib + float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
        }
    }
	
#ifdef _SELF_ILLUM
	float4 l_SelfIllumContrib = tex2D(LightMapSampler, IN.UV2);
	l_LightsContrib = l_LightsContrib + l_SelfIllumContrib;
#endif
	
#ifdef _REFLECTION
	float3 l_CameraPosition = g_ViewInverseMatrix[3].xyz;
	float3 l_CameraToPixel = normalize(l_CameraPosition - l_Position);
	
	float3 l_ReflectVector = reflect(l_CameraToPixel, l_Normal);
	float4 l_EnvironmentColor = texCUBE(ReflectionSampler, l_ReflectVector);
	
	float4 l_PixelColor = l_LightsContrib * l_DiffuseColor + l_EnvironmentColor;
#else
	float4 l_PixelColor = l_LightsContrib * l_DiffuseColor;
#endif
	
	return l_PixelColor;
}

technique MultiLightsTechnique
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderVS();
        PixelShader = compile ps_3_0 RenderPS();
    }
}