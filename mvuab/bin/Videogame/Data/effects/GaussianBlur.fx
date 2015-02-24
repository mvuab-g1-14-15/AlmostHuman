// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/

#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

#define RADIUS  7
#define KERNEL_SIZE (RADIUS * 2 + 1)

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

float weights[KERNEL_SIZE];
float2 offsets[KERNEL_SIZE];

float offsetH[5] = { 0.001454, 0.002765, 0.003874, 0.002525, 0.029236 };
float weightH[5] = { 0.015019, 0.059912, 0.094907, 0.059912, 0.015019 };

float offsetV[5] = { 0.001454, 0.002765, 0.003874, 0.002525, 0.029236 };
float weightV[5] = { 0.015019, 0.059912, 0.094907, 0.059912, 0.015019 };


//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD) : COLOR0
{
    float3 color = float3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < 5; i++)
	{
		color += tex2D(S0LinearSampler, texCoord + float2(offsetH[i], 0.0)).xyz * weightH[i];
		color += tex2D(S0LinearSampler, texCoord - float2(offsetH[i], 0.0)).xyz * weightH[i];
		
		color += tex2D(S0LinearSampler, texCoord + float2(0.0, offsetV[i])).xyz * weightV[i];
		color += tex2D(S0LinearSampler, texCoord - float2(0.0, offsetV[i])).xyz * weightV[i];
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