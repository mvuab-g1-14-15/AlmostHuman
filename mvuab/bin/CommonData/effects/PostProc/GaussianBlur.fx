// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
// http://dev.theomader.com/gaussian-kernel-calculator/

#include "../samplers.fxh"
#include "../globals.fxh"


float GaussianValue(float2 UV, float sigma)
{
    return (1.0 / (2 * PI * sigma * sigma)) * exp(-((UV.x * UV.x + UV.y * UV.y) / (2 * sigma * sigma)));
}

float GaussianValue2(float x, float sigma)
{
    return (1.0 / sqrt(2 * PI * sigma * sigma)) * exp(-((x * x) / (2 * sigma * sigma)));
}

float4 GaussianBlur(sampler2D tex, float2 texCoord, int sz)
{
    float2 l_OffsetTexture = float2(1.0 / g_TextureWidth, 1.0 / g_TextureHeight);
    float3 l_Color = tex2D(tex, texCoord).xyz * 0.50261952;
    
    for(int i = 1; i < sz; i++)
    {
        l_Color += tex2D(tex, texCoord + i * l_OffsetTexture).xyz * GaussianValue2(i, sz);
        l_Color += tex2D(tex, texCoord - i * l_OffsetTexture).xyz * GaussianValue2(i, sz);
    }
    
    return float4(l_Color, 1.0);
}


float4 PS_GaussianBlur(float2 UV : TEXCOORD) : COLOR0
{
/*
	float4 l_DepthMap = tex2D(GUIZMapTextureSampler, UV);
	
	float3 l_CameraPosition = g_InverseViewMatrix[3].xyz;
	float3 l_WorldPosition = GetPositionFromZDepthView(l_DepthMap, float2(0.0, 0.0), g_InverseViewMatrix, g_InverseProjectionMatrix);
	
	float l_Distance = length(l_WorldPosition - l_CameraPosition);
	float l_Blur = 1.0;

	if(l_Distance < g_ZBlurFocalStart) l_Blur = max(l_Distance / g_ZBlurFocalStart, g_ConstantBlur);
	else if(l_Distance > g_ZBlurFocalEnd) l_Blur = max(1.0 - (l_Distance - g_ZBlurFocalEnd) / g_ZBlurEnd, g_ConstantBlur);
	
	
	int pixels = (int)smoothstep( 0, 50, (int)l_Distance );
	(max - min) * (%var) + min
*/
    return GaussianBlur(S0LinearClampSampler, UV, 20);
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