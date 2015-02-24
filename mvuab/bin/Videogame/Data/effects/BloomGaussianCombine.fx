#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

// Shader parameters
float Threshold = 0.15;				// Get the threshold of what brightness level we want to glow
float BloomIntensity = 1.5;			// Controls the Intensity of the bloom texture
float OriginalIntensity = 1.0;		// Controls the Intensity of the original scene texture
float BloomSaturation = 0.5	;		// Saturation amount on bloom
float OriginalSaturation = 0.9;		// Saturation amount on original texture

//-----------------------------------------------------------------------------
// 								FilterBloom
//-----------------------------------------------------------------------------

float4 FilterBloomPS( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = tex2D(S0LinearClampSampler, Tex);
	//return float4(1,0,0,1);
    // Get the bright areas that is brighter than Threshold and return it.
    return saturate((Color - Threshold) / (1 - Threshold));
}


//-----------------------------------------------------------------------------
// 								CombineBloom
//-----------------------------------------------------------------------------

float4 AdjustSaturation(float4 color, float saturation)
{
    // We define gray as the same color we used in the grayscale shader
    float grey = dot(color, float3(0.3, 0.59, 0.11));
    return lerp(grey, color, saturation);
}


float4 FinalBloomPS(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Get our bloom pixel from bloom texture
    float4 bloomColor = tex2D(S0LinearClampSampler, texCoord);

    // Get our original pixel from ColorMap
    float4 originalColor = tex2D(S1LinearClampSampler, texCoord);

    // Adjust color saturation and intensity based on the input variables to the shader
    bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
    originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;

    // make the originalColor darker in very bright areas, avoiding these areas look burned-out
    originalColor *= (1 - saturate(bloomColor));

    // Combine the two images.
    return originalColor + bloomColor;
}

technique BloomFilter
{
    pass p0
    {
        AlphaBlendEnable = false;
        PixelShader = compile ps_3_0 FilterBloomPS();
        ZEnable = false;
    }
}

technique CombineBloomTechnique
{
	pass p0
	{
		PixelShader = compile ps_3_0 FinalBloomPS();
	}
}
