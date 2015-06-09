// http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-in-xna-bloom-and-black-holes--gamedev-9877
// http://xbox.create.msdn.com/en-US/education/catalog/sample/bloom

#include "../samplers.fxh"
#include "../globals.fxh"
#include "bloom.fxh"

//-----------------------------------------------------------------------------
// 								FilterBloom
//-----------------------------------------------------------------------------
float4 FilterBloomPS(float2 Tex : TEXCOORD0) : COLOR
{
	float4 Color = tex2D(S0LinearClampSampler, Tex);
    return saturate((Color - g_BloomThreshold) / (1 - g_BloomThreshold));
}

//-----------------------------------------------------------------------------
// 								CombineBloom
//-----------------------------------------------------------------------------
float4 AdjustSaturation(float4 Color, float Saturation)
{
    float l_Grey = dot(Color, float3(0.3, 0.59, 0.11));
    return lerp(l_Grey, Color, Saturation);
}

float4 FinalBloomPS(float2 texCoord : TEXCOORD0) : COLOR0
{
    float4 bloomColor = tex2D(S0LinearClampSampler, texCoord);
    float4 originalColor = tex2D(S1LinearClampSampler, texCoord);

    bloomColor = AdjustSaturation(bloomColor, g_BloomSaturation) * g_BloomIntensity;
    originalColor = AdjustSaturation(originalColor, g_BloomOriginalSaturation) * g_BloomOriginalIntensity;

    originalColor *= (1 - saturate(bloomColor));
    return originalColor + bloomColor;
}

technique BloomFilter
{
    pass p0
    {
        PixelShader = compile ps_3_0 FilterBloomPS();
    }
}

technique CombineBloomTechnique
{
	pass p0
	{
		PixelShader = compile ps_3_0 FinalBloomPS();
	}
}
