#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

//Albedo (float3) + (float) SpecularFactor => S0XXXSampler
//AmbientLight (float3) + (float) SpecularPow => S1XXXSampler
//Normal (float3) + (float) Not used => S2XXXSampler
//Depth (float4) => S3XXXSampler
	
float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	return float4(0,1,0,1);
	float4 Albedo = tex2D(S0PointSampler, UV); //Albedo (float3) + (float) SpecularFactor
	float4 Ambient = tex2D(S1PointSampler, UV); //AmbientLight (float3) + (float) SpecularPow
	float4 Normal = tex2D(S2PointSampler, UV); //Normal (float3) + (float) Not used
	float4 Depth = tex2D(S3PointSampler, UV); //Depth (float4)
	
	// Obtain the normal of the face and the position
	float3 l_Normal= Normal.xyz;
				
	//float3 l_Position = IN.WorldPosition;
	
	// Init the color of the pixel with the color of the material
	float4 l_PixelColor = float4(Ambient.xyz, 1.0); ;
	
	float4 l_TextureColor = float4(Albedo.xyz, 1.0);
	
	return l_PixelColor * l_TextureColor;
	/*
	for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
		if(g_LightsEnabled[i] == 1) 
        {
			float3 l_LightDirection = l_Position-g_LightsPosition[i];
			float l_Attenuation = DistanceAttenuation(i, l_LightDirection );
			l_LightDirection=normalize(l_LightDirection);
			
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
			float3 l_DiffuseContrib = l_TextureColor*saturate(dot(l_Normal,-l_LightDirection)) * l_Attenuation * g_LightsColor[i];
			float l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * l_Attenuation * g_LightsColor[i];
			l_PixelColor = l_PixelColor + float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
        }
    }
	
	return l_PixelColor;
	
	//return float4(UV.x,UV.y,0,1);*/
}

technique RenderDeferredTechnique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}
