#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

//Albedo (float3) + (float) SpecularFactor => S0XXXSampler
//AmbientLight (float3) + (float) SpecularPow => S1XXXSampler
//Normal (float3) + (float) Not used => S2XXXSampler
//Depth (float4) => S3XXXSampler

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 Albedo = tex2D(S0PointSampler, UV); //Albedo (float3) + (float) SpecularFactor
	float4 Ambient = tex2D(S1PointSampler, UV); //AmbientLight (float3) + (float) SpecularPow
	float4 Normal = tex2D(S2PointSampler, UV); //Normal (float3) + (float) Not used
	float Depth = tex2D(S3PointSampler, UV).x; //Depth (float4)
	
	// Obtain the normal of the face and the position
	float3 l_Normal = Texture2Normal(Normal.xyz);	
	float3 l_WorldPosition = GetPositionFromZDepthView(Depth, UV);
	float3 l_Position = mul( float4(l_WorldPosition.xyz,1.0), g_ViewMatrix ).xyz;
	
	//RETURN DEBUG
	float4 l_LightContrib;
	
	if(g_LightsEnabled[0] == 1) 
	{
		float3 l_LightDirection = l_Position-g_LightsPosition[0];
		float l_Attenuation = DistanceAttenuation(0, l_LightDirection );
		l_LightDirection=normalize(l_LightDirection);
		
		if(OMNI_LIGHT == g_LightsType[0])
		{
		}
		if(DIR_LIGHT == g_LightsType[0])
		{
			l_LightDirection = normalize(g_LightsDirection[0]);
		}
		else if(SPOT_LIGHT == g_LightsType[0])
		{	
			// Modify the distance attenuation with the spot attenuation
			l_Attenuation = l_Attenuation * SpotAttenuation(0, l_LightDirection);
		}
		
		float3 l_Hn=normalize(normalize(g_CameraPosition-l_Position)-l_LightDirection);
		float3 l_DiffuseContrib = saturate(dot(l_Normal,-l_LightDirection)) * l_Attenuation * g_LightsColor[0];
		float l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * l_Attenuation * g_LightsColor[0];
		l_LightContrib = float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
	}
	
	return 0.15*l_LightContrib*l_LightContrib;
}

technique RenderDeferredTechnique {
	pass p0 {
		
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		
		VertexShader = null;
		PixelShader = compile ps_3_0 mainPS();
	}
}
