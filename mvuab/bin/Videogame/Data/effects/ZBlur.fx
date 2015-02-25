#include "globals.fxh"
#include "samplers.fxh"

float g_ZBlurFocalStart = 30;
float g_ZBlurFocalEnd = 40;
float g_ZBlurEnd = 50;

float g_RenderTargetSize = 1.0;
float g_ConstantBlur = 0.4;

float4 ZBlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	float3 l_WorldPosition = GetPositionFromZDepthView(tex2D(S1LinearClampSampler, UV).x, float2(0.0, 0.0), g_ViewInverseMatrix, g_ProjectionInverseMatrix);
	float l_Distance = length(l_WorldPosition - g_ViewInverseMatrix[3].xyz);
	
	float l_Blur = 1.0;
	if(l_Distance < g_ZBlurFocalStart) l_Blur = max(l_Distance / g_ZBlurFocalStart, g_ConstantBlur);
	else if(l_Distance > g_ZBlurFocalEnd) l_Blur = max(1.0 - (l_Distance - g_ZBlurFocalEnd) / g_ZBlurEnd, g_ConstantBlur);
	
	float4 l_Color = GaussianBlur(S0LinearClampSampler, UV) * (1 - l_Blur) + l_Blur * tex2D(S0LinearClampSampler, UV);
	return l_Color;	
}

technique ZBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}