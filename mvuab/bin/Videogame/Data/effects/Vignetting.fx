#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"


float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Color = tex2D(S0LinearSampler, UV);
	float2 l_Offset=float2(cos(g_DeltaTime),sin(g_DeltaTime));
	float2 l_UV=UV+l_Offset;
	float4 l_Vignetting = tex2D(S1LinearSampler, UV);
    float4 l_NoiseColor = tex2D(S2LinearSampler, l_UV*30.5);
    return float4(l_Color.xyz*l_Vignetting.xyz*l_NoiseColor.xyz, l_NoiseColor.a+l_Vignetting.a); 
}

technique VignettingTechnique 
	{
	pass p0 
	{
		CullMode = CCW;
		PixelShader = compile ps_3_0 mainPS();
	}
}