#include "samplers.fxh"
#include "globals.fxh"

//Albedo (float3) + (float) SpecularFactor => S0XXXSampler
//AmbientLight (float3) + (float) SpecularPow => S1XXXSampler
//Normal (float3) + (float) Not used => S2XXXSampler
//Depth (float4) => S3XXXSampler
	
float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	return float4(1,0,0,1);
	float4 Albedo = tex2D(S1PointSampler, UV); //Albedo (float3) + (float) SpecularFactor
	float4 Ambient = tex2D(S2PointSampler, UV); //AmbientLight (float3) + (float) SpecularPow
	
	float4 l_Diffuse = float4(Albedo.xyz, 1);
	float4 l_Ambient = float4(Ambient.xyz,1);
	
	float4 l_Color = l_Diffuse * l_Ambient;
	
	return l_Color;
}

technique RenderDeferredTechnique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}
