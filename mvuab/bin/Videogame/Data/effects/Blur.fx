#include "samplers.fxh"

float4 BlurPS(in float2 UV : TEXCOORD0) : COLOR
{	
	float l_Diff = 0.2;
	float4 l_Color = tex2D(S0LinearSampler, UV) * l_Diff  + tex2D(S1LinearSampler, UV) * (1 - l_Diff);
	
	return l_Color;
}

technique BlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 BlurPS();
	}
}