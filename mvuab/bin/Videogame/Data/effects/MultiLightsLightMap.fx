#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

struct VertexPS
{
    float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
    float3 Normal : TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
};

VertexPS RenderVS(TNORMAL_T2_VERTEX IN)
{
    VertexPS OUT=(VertexPS)0;
    OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.UV=IN.UV;
	OUT.UV2=IN.UV2;
    OUT.Normal=mul(IN.Normal, (float3x3)g_WorldMatrix);
    OUT.WorldPosition=mul(float4(IN.Position,1.0), g_WorldMatrix);
    return OUT;
}

float4 RenderPS(VertexPS IN) : COLOR
{
	// Obtain the normal of the face and the position
	float3 l_Normal= normalize(IN.Normal);
				
	float3 l_Position = IN.WorldPosition;
	
	// Init the color of the pixel with the color of the material
	float4 l_PixelColor = float4(0.0, 0.0, 0.0, 1.0); //tex2D(S0LinearSampler, IN.UV);
	
	/*float3 l_CameraPosition = g_ViewInverseMatrix[3].xyz;
	float3 l_CameraToPixel = normalize(l_CameraPosition - l_Position);
	
	float3 l_ReflectVector = reflect(l_CameraToPixel, l_Normal);
	float4 l_EnvironmentColor = texCUBE(S0LineaWrapSampleCUBE, l_ReflectVector);
	
	l_PixelColor = l_PixelColor + l_EnvironmentColor;*/
	
	for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i] == 1) 
        {
			float3 l_LightDirection = normalize(l_Position-g_LightsPosition[i]);
			float4 l_TextureColor = tex2D(S0LinearSampler, IN.UV);
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
				//l_PixelColor = l_PixelColor * SpotAttenuation(i, l_LightDirection);
            }
			
			float3 l_Hn=normalize(normalize(g_CameraPosition-l_Position)-l_LightDirection);
			float3 l_DiffuseContrib = l_TextureColor*saturate(dot(l_Normal,-l_LightDirection)) * l_Attenuation * g_LightsColor[i];
			float l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * l_Attenuation * g_LightsColor[i];
			l_PixelColor = l_PixelColor + float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
        }
    }
	
	l_PixelColor = l_PixelColor + tex2D(S1LinearSampler, IN.UV2);
	
	return l_PixelColor;
}

technique MultiLightsLightMapTechnique
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderVS();
        PixelShader = compile ps_3_0 RenderPS();
    }
}