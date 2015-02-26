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

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD) : COLOR0
{
	float3 l_Color=float3(0.0, 0.0, 0.0);
	float2 l_OffsetTexture=float2(1/800.0, 1/600.0);
	
	l_Color += tex2D(S0LinearClampSampler, texCoord) * 0.204164;
	l_Color += tex2D(S0LinearClampSampler, texCoord+l_OffsetTexture*1.407333) * 0.304005;
	l_Color += tex2D(S0LinearClampSampler, texCoord-l_OffsetTexture*1.407333) * 0.304005;
	l_Color += tex2D(S0LinearClampSampler, texCoord+l_OffsetTexture*3.294215) * 0.093913;
	l_Color += tex2D(S0LinearClampSampler, texCoord-l_OffsetTexture*3.294215) * 0.093913;
	
	return float4(l_Color, 1.0);
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