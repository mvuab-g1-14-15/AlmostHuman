#include "samplers.fxh"
#include "globals.fxh"

float4 LensFlarePS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 outColor = float4(0.0, 0.0, 0.0, 1.0);
	float dist[4] = {0.05, 0.16, 0.25, 0.45};
	
	float2 center = float2(0.5, 0.5);
    float4 LightPos = mul(float4(g_LightsPosition[0], 1.0), g_ViewProjMatrix);
	
	float2 LightScreenPos = float2((LightPos.x + 1) / 2.0, ((1 - LightPos.y ) / 2.0));
	float len = length(LightScreenPos - UV);
	if (len < 0.05)
		outColor = float4(1.0, 0.0, 0.0, 1.0);
	
	return outColor;
	/*
	if ((LightPos.x < 0 || LightPos.x > 1) || (LightPos.y < 0 || LightPos.y > 1))
		float4(0.0, 0.0, 0.0, 1.0);
		
		
    float2 dir = normalize(LightPos - center);
	
	float dist2light = length(center - LightPos);

	for (int i=-3; i<4; ++i)
	{
		float2 actual_point = sign(i)*dir*dist[abs(i)] + center;
		float len = length(abs(actual_point - UV));
		if (len < dist[abs(i)]*0.15)
			outColor = float4(1.0, 1.0, 1.0, 1.0);
	}
	
	float len = length(LightPos - UV);
	if (len < 0.01)
		outColor = float4(1.0, 1.0, 1.0, 1.0);
	
	return outColor;*/
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