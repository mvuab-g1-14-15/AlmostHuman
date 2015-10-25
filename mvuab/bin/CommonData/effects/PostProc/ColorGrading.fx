// https://docs.unrealengine.com/latest/INT/Engine/Rendering/PostProcessEffects/ColorGrading/index.html
// http://gamedev.stackexchange.com/questions/61738/color-grading-shaders-and-3d-texture
// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter24.html
// https://www.youtube.com/watch?v=rfQ8rKGTVlg#t=26m00s
// http://kpulv.com/359/Dev_Log__Color_Grading_Shader/
// https://udn.epicgames.com/Three/ColorGrading.html

#include "../globals.fxh"
#include "../samplers.fxh"

// Modificar ROGER Valores de 0 a 1
float g_MonochromeColorGrading = 0.0;
float g_BrownSepiaColorGrading = 0.1;
float g_GreenSepiaColorGrading = 0.3;
float g_BlueSepiaColorGrading = 0.3;
float g_ContrastColorGrading = 0.5;
float g_BrightnessColorGrading = 0.0;
float g_ColorColorGrading = 1.0;   
// Modificar ROGER

float4 ColorGrading(float4 _Color)
{
	//-------Color Matrices for Color Correction--------------
	float4x4 gray = {0.299,0.587,0.184,0,
					0.299,0.587,0.184,0,
					0.299,0.587,0.184,0,
					0,0,0,1};
	float4x4 sepia = {0.299,0.587,0.184,0.1,
					0.299,0.587,0.184,0.018,
					0.299,0.587,0.184,-0.090,
					0,0,0,1};
	float4x4 sepia2 = {0.299,0.587,0.184,-0.090,
						0.299,0.587,0.184,0.018,
						0.299,0.587,0.184,0.1,
						0,0,0,1};
	float4x4 sepia3 = {0.299,0.587,0.184,-0.090,
						0.299,0.587,0.184,0.1,
						0.299,0.587,0.184,0.1,
						0,0,0,1};
	float4x4 sepia4 = {0.299,0.587,0.184,-0.090,
						0.299,0.587,0.184,0.018,
						0.1299,0.587,0.184,0.1,
						0,0,0,1};
						
	float3 monochrome = (_Color.r * 0.3f + _Color.g * 0.59f + _Color.b * 0.11f);
	float4 monochrome4 = float4(monochrome,1);
	float4 result2 = _Color;
	float4 brownsepia = mul(sepia,result2);
	float4 greensepia = mul(sepia3,result2);
	float4 bluesepia = mul(sepia2,result2);
	float4 combine = (brownsepia *g_BrownSepiaColorGrading ) + (greensepia *g_BrownSepiaColorGrading )+ (bluesepia * g_BlueSepiaColorGrading )+ (monochrome4 * g_MonochromeColorGrading)+(g_ColorColorGrading * result2);
	return (combine  * g_ContrastColorGrading)  + g_BrightnessColorGrading;
}
  
float4 ColorGradingPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Color= tex2D(S0LinearSampler, UV);
	return ColorGrading(l_Color);
}

technique ColorGradingTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		//CullMode = CCW;
		PixelShader = compile ps_3_0 ColorGradingPS();
	}
}
