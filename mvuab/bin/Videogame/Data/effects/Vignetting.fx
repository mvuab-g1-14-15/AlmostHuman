#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
    float4 l_Color = tex2D(S0LinearSampler, UV);	
	float4 l_Vignetting = tex2D(S1LinearSampler, UV);
    float4 l_NoiseColor = tex2D(S2LinearSampler, UV*30);
	return float4(l_Color.xyz*l_NoiseColor.xyz*(1-l_Vignetting.a), 1.0);
}

technique VignettingTechnique 
	{
	pass p0 
	{
		CullMode = CCW;
		PixelShader = compile ps_3_0 mainPS();
	}
}