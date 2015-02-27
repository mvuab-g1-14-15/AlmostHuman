// http://en.wikipedia.org/wiki/Relative_luminance
// http://www.pouet.net/topic.php?which=9841

#include "samplers.fxh"
#include "globals.fxh"

float g_Gamma = 1.0;
float g_Saturation = 1.0;

float g_Constrast = 1.0;
float g_Brightness = 0.0;


float Luminance(float3 l_Color)
{
    return dot(l_Color, float3(0.2126, 0.7152, 0.0722));
}

// Gamma (1 = normal, <1 = brighter, >1 = darker)
float3 Gamma(float3 l_Color, float l_Gamma)
{
    return pow(l_Color, l_Gamma);
}

// Saturation (0 = b+w, 1 = normal, >1 = high saturation)
float3 Saturation(float3 l_Color, float l_Saturation)
{
    float l_Luminance = Luminance(l_Color);
    return (l_Color - l_Luminance) * l_Saturation + l_Luminance;
}

// Brightness (0 = no brightness, 1 = normal, >1 = brighter)
// Contrast (0 = flat grey, 1 = normal, >1 = higher contrast)
float3 ContrastBrightness(float3 l_Color, float l_Constrast, float l_Brightness)
{
    return (l_Color - 0.5) * l_Constrast + 0.5 + l_Brightness;
}

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
    float3 l_Color =  tex2D(S0LinearSampler, UV).xyz;
    
    l_Color = Gamma(l_Color, g_Gamma);
    l_Color = Saturation(l_Color, g_Saturation);
    l_Color = ContrastBrightness(l_Color, g_Constrast, g_Brightness);
    
	return float4(l_Color, 1.0);
}

technique DrawQuadSampler0Technique
{
	pass p0
    {
		PixelShader = compile ps_3_0 mainPS();
	}
}
