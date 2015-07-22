// http://theorangeduck.com/page/pure-depth-ssao
// http://blog.evoserv.at/index.php/2012/12/hemispherical-screen-space-ambient-occlusion-ssao-for-deferred-renderers-using-openglglsl/

#include "../samplers.fxh"
#include "../globals.fxh"

float4 ps_ssao(in float2 UV : TEXCOORD0) : COLOR
{
    const int    g_samples = 16;
    const float  g_distanceThreshold = 5.0f;
    const float2 g_filterRadius = float2(1.5f / 1920.0f, 1.5f / 1080.0f);
    
	float2 sample_sphere[g_samples] = 
    {
        float2( -0.94201624,  -0.39906216 ), float2(  0.94558609,  -0.76890725 ),
        float2( -0.094184101, -0.92938870 ), float2(  0.34495938,   0.29387760 ),
        float2( -0.91588581,   0.45771432 ), float2( -0.81544232,  -0.87912464 ),
        float2( -0.38277543,   0.27676845 ), float2(  0.97484398,   0.75648379 ),
        float2(  0.44323325,  -0.97511554 ), float2(  0.53742981,  -0.47373420 ),
        float2( -0.26496911,  -0.41893023 ), float2(  0.79197514,   0.19090188 ),
        float2( -0.24188840,   0.99706507 ), float2( -0.81409955,   0.91437590 ),
        float2(  0.19984126,   0.78641367 ), float2(  0.14383161,  -0.14100790 )
	};

	float l_Depth = tex2D(S0LinearSampler, UV).x;
	float3 l_PixelNormal = normalize(tex2D(S1LinearSampler, UV).xyz * 2.0f - 1.0f);
    float3 l_PixelPos = GetPositionFromZDepthView(l_Depth, UV, g_ViewInverseMatrix, g_ProjectionInverseMatrix);
    
	float occlusion = 0.0;
	for(int i = 0; i < g_samples; i++)
	{
		float2 l_TexCoord = UV + sample_sphere[i] * g_filterRadius; 
		float l_SampleDepth = tex2D(S0LinearSampler, l_TexCoord).r;
        
        float3 l_SamplePos = GetPositionFromZDepthView(l_SampleDepth, l_TexCoord, g_ViewInverseMatrix, g_ProjectionInverseMatrix);
        float3 l_SampleDir = normalize(l_SamplePos - l_PixelPos);
        
        float l_Angle = dot(l_PixelNormal, l_SampleDir);        
        float l_Distance = distance(l_PixelPos, l_SamplePos);
        
        float a = 1.0 - smoothstep(g_distanceThreshold, g_distanceThreshold * 2, l_Distance);
		occlusion += a * l_Angle;
	}
    
    occlusion = occlusion / g_samples;
	return float4(occlusion, occlusion, occlusion, 1.0);
}

technique SSAOTechnique
{
	pass p0
	{
		//AlphaBlendEnable = true;
        //BlendOp = RevSubtract;
        //DestBlend = one;
		//SrcBlend = one;
		
		PixelShader = compile ps_3_0 ps_ssao();
	}
}