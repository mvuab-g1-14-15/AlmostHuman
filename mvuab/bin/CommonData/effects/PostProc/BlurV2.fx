#include "../vertex_types.fxh"
#include "../samplers.fxh"
#include "../globals.fxh"

float4 PS_BlurHorizontal( float2 UV : TEXCOORD0 ) : COLOR0
{
    float Color = 0.0f;

    Color += tex2D(S0LinearSampler, float2(UV.x - 3.0*g_TextureWidth, UV.y)) * 0.09f;
    Color += tex2D(S0LinearSampler, float2(UV.x - 2.0*g_TextureWidth, UV.y)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x - g_TextureWidth, UV.y)) * 0.18f;
    Color += tex2D(S0LinearSampler, UV) * 0.24f;
    Color += tex2D(S0LinearSampler, float2(UV.x + g_TextureWidth, UV.y)) * 0.18f;
    Color += tex2D(S0LinearSampler, float2(UV.x + 2.0*g_TextureWidth, UV.y)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x + 3.0*g_TextureWidth, UV.y)) * 0.09f;

    return Color;
}

float4 PS_BlurVertical(in float2 UV : TEXCOORD0 ) : COLOR0
{
    float Color = 0.0f;

    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - 3.0*g_TextureHeight)) * 0.09f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - 2.0*g_TextureHeight)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - g_TextureHeight)) * 0.18f;
    Color += tex2D(S0LinearSampler, UV) * 0.24f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + g_TextureHeight)) * 0.18f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + 2.0*g_TextureHeight)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + 3.0*g_TextureHeight)) * 0.09f;

    return Color;
}


technique BlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 PS_BlurHorizontal();
	}
	
	pass p1
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 PS_BlurVertical();
	}
}