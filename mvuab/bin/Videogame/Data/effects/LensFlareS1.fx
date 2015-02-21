#include "samplers.fxh"
#include "globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	float l_Depth = tex2D(S1PointSampler, UV).x;
	float3 l_ScreenColor = tex2D(S0PointSampler, UV).xyz;
	float l_SumChannels = l_ScreenColor.x+l_ScreenColor.y+l_ScreenColor.z;
	float l_GreyColor = l_SumChannels/3;
	float l_Spec = l_GreyColor == 1;
	return float4(l_Spec*l_Depth, l_Spec*l_Depth, l_Spec*l_Depth, 1);
}

technique LensFlareS1Technique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 LensFlarePS();
	}
}