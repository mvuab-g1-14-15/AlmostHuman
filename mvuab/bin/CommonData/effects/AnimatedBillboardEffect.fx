#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float3 scrollSpeeds 	= float3(1.3f, 1.9f, 2.1f);
float3 scales 			= float3(1.f, 2.f, 3.f);
float2 distortion1 		= float2(0.1f, 0.2f);
float2 distortion2 		= float2(0.1f, 0.3f);
float2 distortion3 		= float2(0.1f, 0.1f);
float distortionScale 	= 0.8f;
float distortionBias 	= 0.5f;

struct TT1_VERTEX_BLASTER_PS
{
	float4 Position 		: POSITION;
    float2 UV 				: TEXCOORD0;
	float2 UV2              : TEXCOORD1;
    float2 UV3              : TEXCOORD2;
    float2 UV4              : TEXCOORD3;
};

TT1_VERTEX_BLASTER_PS mainBlasterVS(TT1_VERTEX_VS IN)
{
	TT1_VERTEX_BLASTER_PS OUT=(TT1_VERTEX_BLASTER_PS)0;
	
	float3 rightVector 	= normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0] ));
	float3 upVector 	= normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1] ));
	float3 viewVector	= normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2] ));
	
#if defined( ALIGNED_TO_UP_AXIS )
	upVector 			= float3(0,1,0);
#endif
	
	float3 position    	= (IN.Position.x*rightVector+IN.Position.z*upVector) * g_Size;
	OUT.Position		= mul( float4( position.xyz, 1), g_WorldViewProj);

	// Get the correct UV's for the billboard
	float u = IN.UV.x;
	if( g_FlipUVHorizontal ) u = 1.0 - u;
	float v = IN.UV.y;
	if( g_FlipUVVertical ) v = 1.0 - v;
	OUT.UV				= float2( u, v );
	
#if defined( FIRE )
	// Other UV's
	OUT.UV2 	= OUT.UV * scales.x;
	OUT.UV2.y 	= OUT.UV2.y + (g_DeltaTime * scrollSpeeds.x);
	OUT.UV3 	= OUT.UV * scales.y;
	OUT.UV3.y 	= OUT.UV3.y + (g_DeltaTime * scrollSpeeds.y);
	OUT.UV4 	= OUT.UV * scales.z;
	OUT.UV4.y 	= OUT.UV4.y + (g_DeltaTime * scrollSpeeds.z);
#endif

#if defined( BLASTER )
	// Other UV's
	OUT.UV2 	= OUT.UV + g_DeltaTime;
#endif
	
    return OUT;
}

//Dest.rgb*One + Source.rgb*One

float4 mainBlasterPS(TT1_VERTEX_BLASTER_PS IN) : COLOR
{
	float4 noise1 = tex2D(S1LinearWrapSampler, IN.UV2);
	float4 noise2 = tex2D(S1LinearWrapSampler, IN.UV3);
	float4 noise3 = tex2D(S1LinearWrapSampler, IN.UV4);
	
	noise1 = (noise1 - 0.5f) * 2.f;
	noise2 = (noise2 - 0.5f) * 2.f;
	noise3 = (noise3 - 0.5f) * 2.f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	float4 finalNoise = noise1 + noise2 + noise3;

	float perturb = ((1.0f - IN.UV.y) * distortionScale) + distortionBias;

	float2 noiseCoords = (finalNoise.xy * perturb) + IN.UV.xy;
	
#if defined( BLASTER )
	float4 fireColor   = tex2D(S0LinearWrapSampler, IN.UV2);
	float4 alphaColor  = tex2D(S2LinearClampSampler, IN.UV);
	return fireColor * alphaColor * noise1;
#else
	float4 fireColor   = tex2D(S0LinearClampSampler, noiseCoords.xy);
	float4 alphaColor  = tex2D(S2LinearClampSampler, noiseCoords.xy);
	return fireColor * alphaColor;
#endif
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		VertexShader = compile vs_3_0 mainBlasterVS();
		PixelShader = compile ps_3_0 mainBlasterPS();
	}
}
