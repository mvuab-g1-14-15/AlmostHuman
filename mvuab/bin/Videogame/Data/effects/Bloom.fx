#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"


// Controls the Intensity of the original scene texture
float OriginalIntensity = 1.0;
 
// Saturation amount on bloom
float BloomSaturation = 1.0;
 
// Saturation amount on original scene
float OriginalSaturation = 1.0;

float4 AdjustSaturation(float4 color, float saturation)
{
    // We define gray as the same color we used in the grayscale shader
    float grey = dot(color, float3(0.3, 0.59, 0.11));
   
    return lerp(grey, color, saturation);
}

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{

	// Get our bloom pixel from bloom texture
	float4 bloomColor = tex2D(S0PointSampler, UV);

	// Get our original pixel from ColorMap
	float4 originalColor = tex2D(S1PointSampler, UV);

	// Adjust color saturation and intensity based on the input variables to the shader
	bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
	originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;

	// make the originalColor darker in very bright areas, avoiding these areas look burned-out
	originalColor *= (1 – saturate(bloomColor));

	// Combine the two images.
	return originalColor + bloomColor;
}

technique Bloom {
	pass p0 {
		PixelShader = compile ps_2_0 PixelShader();
	}
}
