#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

// Get the threshold of what brightness level we want to glow
float Threshold = 0.8;
 
 
float4 PixelShader(float2 UV : TEXCOORD0) : COLOR0
{
	//return float4(1,0,0,1);
    float4 Color = tex2D(S0PointSampler, UV);
	//return float4(Color.xyz, 1.0);
   
    // Get the bright areas that is brighter than Threshold and return it.
	return saturate((Color - Threshold)/(1 - Threshold));
}
 
 
technique BloomTechnique
{
    pass P0
    {
		// A post process shader only needs a pixel shader.
		PixelShader = compile ps_3_0 PixelShader();
    }
}