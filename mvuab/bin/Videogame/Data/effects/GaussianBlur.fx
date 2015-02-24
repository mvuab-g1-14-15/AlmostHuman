// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
// http://dev.theomader.com/gaussian-kernel-calculator/


#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"


//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float2 l_OffsetTexture=float2(1/800.0, 1/600.0);
float g_GaussianOffsets[5]={0.0, 1.0, 2.0, 3.0, 4.0};
float g_GaussianWeights[5]={0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD) : COLOR0
{
	float3 color=float3(0,0,0);
	
	color += tex2D(S0LinearClampSampler, texCoord) * 0.204164;
	color += tex2D(S0LinearClampSampler, texCoord+l_OffsetTexture*1.407333) * 0.304005;
	color += tex2D(S0LinearClampSampler, texCoord-l_OffsetTexture*1.407333) * 0.304005;
	color += tex2D(S0LinearClampSampler, texCoord+l_OffsetTexture*3.294215) * 0.093913;
	color += tex2D(S0LinearClampSampler, texCoord-l_OffsetTexture*3.294215) * 0.093913;
	
	return float4(color, 1.0);
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique GaussianBlur
{
    pass
    {
        PixelShader = compile ps_3_0 PS_GaussianBlur();
    }
}