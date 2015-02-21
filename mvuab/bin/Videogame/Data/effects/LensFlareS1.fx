#include "samplers.fxh"
#include "globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 LightPos = float2(0,0);
	float2 vecPixelLight = normalize(UV-LightPos);
	float2 vecCenterLight = normalize(float2(0.5,0.5)-LightPos);
	
	float2 diff = vecCenterLight-vecPixelLight;
	
	float4 l_Color = float4(0,0,0,1);
	if (diff.x < 0.02 && diff.x > 0.005)
		l_Color = float4(1,1,1,1);
	return l_Color;
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