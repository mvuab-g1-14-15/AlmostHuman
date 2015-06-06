#include "../samplers.fxh"
#include "../globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	return tex2D(S0LinearSampler, UV);
}

technique LensFlareS1Technique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		
		PixelShader = compile ps_3_0 LensFlarePS();
	}
}