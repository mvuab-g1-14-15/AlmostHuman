#include "samplers.fxh"
#include "globals.fxh"

//Albedo (float3) + (float) SpecularFactor => S0XXXSampler
//AmbientLight (float3) + (float) SpecularPow => S1XXXSampler
//Normal (float3) + (float) Not used => S2XXXSampler
//Depth (float4) => S3XXXSampler
	
float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	return tex2D(S0PointSampler, UV);
}

technique InitRenderDeferredTechnique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}
