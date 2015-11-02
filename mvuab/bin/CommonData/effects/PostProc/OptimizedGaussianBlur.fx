#include "../vertex_types.fxh"
#include "../samplers.fxh"
#include "../globals.fxh"

int blurSizeY = 5;
int blurSizeX = 5;

// Blurs using a 5x5 filter kernel
float4 BlurFunction5x5(float2 UV : TEXCOORD) : COLOR0
{
  return (
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   	   -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      -2.0f / g_TextureHeight)) +

    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                          -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      -1.0f / g_TextureHeight)) +

    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     0)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     0)) +
    tex2D(S0LinearSampler, UV + float2(0,                          0)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      0)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      0)) +

    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                          1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      1.0f / g_TextureHeight)) +

    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                          2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      2.0f / g_TextureHeight))
  ) / 25;
}

// Blurs using a 7x7 filter kernel
float4 BlurFunction7x7(float2 UV : TEXCOORD) : COLOR0
{
	//return tex2D(S0LinearSampler, UV );
  return (
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      -3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      -3.0f / g_TextureHeight)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      -2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      -2.0f / g_TextureHeight)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      -1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      -1.0f / g_TextureHeight)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     0)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     0)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     0)) +
    tex2D(S0LinearSampler, UV + float2(0,                   0)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      0)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      0)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      0)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      1.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      1.0f / g_TextureHeight)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      2.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      2.0f / g_TextureHeight)) +
 
    tex2D(S0LinearSampler, UV + float2(-3.0f / g_TextureWidth,     3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-2.0f / g_TextureWidth,     3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(-1.0f / g_TextureWidth,     3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(0,                   3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(1.0f / g_TextureWidth,      3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(2.0f / g_TextureWidth,      3.0f / g_TextureHeight)) +
    tex2D(S0LinearSampler, UV + float2(3.0f / g_TextureWidth,      3.0f / g_TextureHeight))
  ) / 49;
}

float4 PS_BlurHorizontal( float2 UV : TEXCOORD ) : COLOR0
{
    float Color = 0.0f;

    Color += tex2D(S0LinearSampler, float2(UV.x - 3.0*blurSizeX, UV.y)) * 0.09f;
    Color += tex2D(S0LinearSampler, float2(UV.x - 2.0*blurSizeX, UV.y)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x - blurSizeX, UV.y)) * 0.18f;
    Color += tex2D(S0LinearSampler, UV) * 0.24f;
    Color += tex2D(S0LinearSampler, float2(UV.x + blurSizeX, UV.y)) * 0.18f;
    Color += tex2D(S0LinearSampler, float2(UV.x + 2.0*blurSizeX, UV.y)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x + 3.0*blurSizeX, UV.y)) * 0.09f;

    return Color;
}

float4 PS_BlurVertical( float2 UV : TEXCOORD ) : COLOR0
{
    float Color = 0.0f;

    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - 3.0*blurSizeY)) * 0.09f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - 2.0*blurSizeY)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y - blurSizeY)) * 0.18f;
    Color += tex2D(S0LinearSampler, UV) * 0.24f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + blurSizeY)) * 0.18f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + 2.0*blurSizeY)) * 0.11f;
    Color += tex2D(S0LinearSampler, float2(UV.x, UV.y + 3.0*blurSizeY)) * 0.09f;

    return Color;
}

float4 BlurFunction(float2 UV : TEXCOORD) : COLOR0
{
	return PS_BlurHorizontal( UV ) + PS_BlurVertical( UV );
}

technique TECHNIQUE_NAME
{
    pass p0
    {
        PixelShader = compile ps_3_0 BlurFunction7x7();
    }
}
