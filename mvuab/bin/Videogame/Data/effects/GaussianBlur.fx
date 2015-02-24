// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
// http://dev.theomader.com/gaussian-kernel-calculator/


#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

#define MAX_STEPS 10
#define PI 3.14159265359

float offset[MAX_STEPS];
float weight[MAX_STEPS];

float GaussianValue(float2 UV, float sigma)
{
	float g = (1.0 / (2 * PI * sigma * sigma)) * exp(-((UV.x * UV.x + UV.y * UV.y) / (2 * sigma * sigma)));
	return g;
}

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD) : COLOR0
{
    float3 color = float3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < MAX_STEPS; i++)
	{
		offset[i] = GaussianValue(texCoord, 4.0);
		weight[i] = GaussianValue(texCoord, 3.0);
	}
	
	for (int i = 0; i < MAX_STEPS; i++)
	{
		color += tex2D(S0LinearSampler, texCoord + float2(offset[i], 0.0)).xyz * weight[i];
		color += tex2D(S0LinearSampler, texCoord - float2(offset[i], 0.0)).xyz * weight[i];
		
		color += tex2D(S0LinearSampler, texCoord + float2(0.0, offset[i])).xyz * weight[i];
		color += tex2D(S0LinearSampler, texCoord - float2(0.0, offset[i])).xyz * weight[i];
	}

    return float4(color, 1.0);
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique GaussianBlur
{
    pass
    {
        PixelShader = compile ps_2_0 PS_GaussianBlur();
    }
}