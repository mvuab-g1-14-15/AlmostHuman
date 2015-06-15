// http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-in-xna-bloom-and-black-holes--gamedev-9877
// http://xbox.create.msdn.com/en-US/education/catalog/sample/bloom
// https://digitalerr0r.wordpress.com/2009/10/04/xna-shader-programming-tutorial-24-bloom/

#include "../samplers.fxh"
#include "../globals.fxh"
#include "bloom.fxh"

float4 BloomFilterPS(float2 UV : TEXCOORD0) : COLOR
{
	return saturate((tex2D(S0LinearClampSampler, UV) - g_BloomThreshold) / (1 - g_BloomThreshold)); // +tex2D(S1Sampler, Textura glow); //Parte GLOW
}

float4 AdjustSaturation(float4 Color, float Saturation)
{
    float l_Grey = dot(Color, float3(0.3, 0.59, 0.11));
    return lerp(l_Grey, Color, Saturation);
}

float4 BloomEffectGaussianCombinePS(float2 texCoord : TEXCOORD0) : COLOR0
{
    float4 bloomColor = tex2D(S0LinearClampSampler, texCoord);
    float4 originalColor = tex2D(S1LinearClampSampler, texCoord);

    bloomColor = AdjustSaturation(bloomColor, g_BloomSaturation) * g_BloomIntensity;
    originalColor = AdjustSaturation(originalColor, g_BloomOriginalSaturation) * g_BloomOriginalIntensity;

    originalColor *= (1 - saturate(bloomColor));
    return originalColor + bloomColor;
}

technique TECHNIQUE_NAME
{
    pass p0
    {
	#if defined( STAGE_COMBINE )
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
	#endif
        PixelShader = compile ps_3_0 PS_NAME();
    }
}
