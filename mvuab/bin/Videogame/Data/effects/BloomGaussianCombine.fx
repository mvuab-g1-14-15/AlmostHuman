// http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-in-xna-bloom-and-black-holes--gamedev-9877
// http://xbox.create.msdn.com/en-US/education/catalog/sample/bloom

#include "samplers.fxh"
#include "globals.fxh"

// Shader parameters
float Threshold = 0.25;				// Get the threshold of what brightness level we want to glow

float BloomIntensity = 2;			// Controls the Intensity of the bloom texture
float OriginalIntensity = 1.0;		// Controls the Intensity of the original scene texture

float BloomSaturation = 1.5	;		// Saturation amount on bloom
float OriginalSaturation = 1.0;		// Saturation amount on original texture

//-----------------------------------------------------------------------------
// 								FilterBloom
//-----------------------------------------------------------------------------
float4 FilterBloomPS(float2 Tex : TEXCOORD0) : COLOR
{
	float4 Color = tex2D(S0LinearClampSampler, Tex);
    return saturate((Color - Threshold) / (1 - Threshold));
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

    bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
    originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;

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
