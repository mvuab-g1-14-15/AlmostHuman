#include "samplers.fxh"
#include "globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 UVBias1 = float2(0.1, 0.1);
	float2 UVBias2 = float2(-0.1, -0.1);
	
	float2 UV1 = UV + UVBias1;
	float2 UV2 = UV + UVBias2;
	
	float l_ColorBias1 = tex2D(S0PointSampler, UV1).x;
	float l_ColorBias2 = tex2D(S0PointSampler, UV2).x;
	
	float4 l_Color = float4(0,0,0,1);
	if (l_ColorBias1 > 0.9 || l_ColorBias2 > 0.9)
		l_Color = float4(1,1,1,1);
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