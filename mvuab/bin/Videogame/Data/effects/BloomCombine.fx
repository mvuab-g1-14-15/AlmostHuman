#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

// This combines the bloom texture with the original scene texture.
// BloomIntensity, OriginalIntensity, BloomSaturation and OriginalSaturation is used
// to control the blooming effect.
// This shader is based on the example in creators.xna.com, where I learned this technique.
//Source information: https://digitalerr0r.wordpress.com/2009/10/04/xna-shader-programming-tutorial-24-bloom/

float4 AdjustSaturation(float4 color, float saturation)
{
    // We define gray as the same color we used in the grayscale shader
    float grey = dot(color, float3(0.02, 0.59, 0.11));
   
    return lerp(grey, color, saturation);
}

float4 BloomPS(in float2 UV : TEXCOORD0) : COLOR
{
	//return float4(1,1,0,1);
	// Get our bloom pixel from bloom texture
	float4 bloomColor = tex2D(S0PointSampler, UV);

	// Get our original pixel from ColorMap
	float4 originalColor = tex2D(S0PointSampler, UV);

	// Adjust color saturation and intensity based on the input variables to the shader
	bloomColor = AdjustSaturation(bloomColor, g_BloomSaturation) * g_BloomIntensity;

	originalColor = AdjustSaturation(originalColor, g_OriginalSaturation) * g_OriginalIntensity;
	
	// make the originalColor darker in very bright areas, avoiding these areas look burned-out
	originalColor = originalColor*(1 - saturate(bloomColor));

	// Combine the two images.
	return originalColor + bloomColor;
}

technique BloomCombineTechnique {
	pass p0 {

		// A post process shader only needs a pixel shader.
		PixelShader = compile ps_3_0 BloomPS();
	}
}
