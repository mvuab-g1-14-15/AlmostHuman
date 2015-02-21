#include "samplers.fxh"
#include "globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	float distances[5] = {0.1, 0.2, 0.25, 0.6, 0.8};
	
	float distCenter = length(float2(0.5,0.5)-UV);
	
	float4 l_Color = float4(0,0,0,0);
	for (int i=0; i<5; ++i)
	{
		float diff = distCenter - distances[i];
		if (diff > 0.0005 && diff < 0.025 && tex2D(S0PointSampler, UV).x == 1)
			l_Color = float4(1,1,0,1);
	}
	return l_Color;
}

technique LensFlareS2Technique
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