// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
// http://dev.theomader.com/gaussian-kernel-calculator/

#include "../samplers.fxh"
#include "../globals.fxh"

float g_BlurOffset = 20.0;
float g_GaussianDivValue = 6.0;
float PI = 3.14159265359;

float weights[8] = {0.3626837833783620,0.3626837833783620,0.3137066458778873, 0.3137066458778873, 0.2223810344533745, 0.2223810344533745, 0.1012285362903763, 0.1012285362903763};

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
		// TOP ROW
		l_Color += tex2D(tex, texCoord + i * float2(-l_OffsetTexture.x, -l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[0]; // LEFT
		l_Color += tex2D(tex, texCoord - i * float2(0, -l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[1];				   // MIDDLE
		l_Color += tex2D(tex, texCoord + i * float2(l_OffsetTexture.x, -l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[2];  // RIGHT
		
		// MIDDLE
		l_Color += tex2D(tex, texCoord + i * float2(-l_OffsetTexture.x, 0)).xyz * GaussianValue2(i, sz) * weights[3];  			       // LEFT
		l_Color += tex2D(tex, texCoord + i * float2(l_OffsetTexture.x, 0)).xyz * GaussianValue2(i, sz) * weights[4];  			       // RIGHT
		
		//BOTTOM
        l_Color += tex2D(tex, texCoord + i * float2(-l_OffsetTexture.x, l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[5];  // LEFT
		l_Color += tex2D(tex, texCoord + i * float2(0, l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[6];                   // MIDDLE
		l_Color += tex2D(tex, texCoord - i * float2(l_OffsetTexture.x, l_OffsetTexture.y)).xyz * GaussianValue2(i, sz) * weights[7];   // LEFT
    }
    
    return float4(l_Color, 1.0);
}

float4 BlurFunction3x3(float2 UV : TEXCOORD) : COLOR0
{
	float2 l_OffsetTexture = float2(1.0 / g_TextureWidth, 1.0 / g_TextureHeight);
	// TOP ROW
	float4 s11 = tex2D(S0LinearClampSampler, UV + float2(-g_BlurOffset / g_TextureWidth, -g_BlurOffset / g_TextureHeight));     // LEFT
	float4 s12 = tex2D(S0LinearClampSampler, UV + float2(0, -g_BlurOffset / g_TextureHeight));              			// MIDDLE
	float4 s13 = tex2D(S0LinearClampSampler, UV + float2(1.0f / g_TextureWidth, -g_BlurOffset / g_TextureHeight)); 		// RIGHT

	// MIDDLE ROW                           
	float4 s21 = tex2D(S0LinearClampSampler, UV + float2(-g_BlurOffset / g_TextureWidth, 0));      // LEFT
	float4 col = tex2D(S0LinearClampSampler, UV);                                          // DEAD CENTER
	float4 s23 = tex2D(S0LinearClampSampler, UV + float2(-g_BlurOffset / g_TextureWidth, 0));      // RIGHT

	// LAST ROW                              
	float4 s31 = tex2D(S0LinearClampSampler, UV + float2(-g_BlurOffset / g_TextureWidth, g_BlurOffset / g_TextureHeight)); // LEFT
	float4 s32 = tex2D(S0LinearClampSampler, UV + float2(0, g_BlurOffset / g_TextureHeight));                      // MIDDLE
	float4 s33 = tex2D(S0LinearClampSampler, UV + float2(g_BlurOffset / g_TextureWidth, g_BlurOffset / g_TextureHeight));  // RIGHT

	// Average the color with surrounding samples
	col = ( (col + s11 + s12 + s13 + s21 + s23 + s31 + s32 + s33) / 9 );
	return col;
}


float4 PS_GaussianBlur(float2 UV : TEXCOORD) : COLOR0
{
/*
	float4 l_DepthMap = tex2D(S1LinearClampSampler, UV);
	float3 l_WorldPosition = GetPositionFromZDepthView( l_DepthMap, float2(0.0, 0.0), g_ViewInverseMatrix, g_ProjectionInverseMatrix );
	float l_Distance = length(l_WorldPosition - g_CameraPosition);
	int pixels = 10;
	
	if( l_Distance > 10.0f )
	{
		pixels = 1;
	}
*/
	//return tex2D(S0LinearClampSampler, UV);
	return GaussianBlur(S0LinearClampSampler, UV, 50);
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