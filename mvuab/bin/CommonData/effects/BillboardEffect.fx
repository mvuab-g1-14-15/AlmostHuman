#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TT1_VERTEX_PS mainBillboardVS(TT1_VERTEX_VS IN)
{
	float3 position = float3(0,0,0);
#if defined( NOT_ALIGNED )
	position = IN.Position;
#else
	float3 rightVector 	= normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0] ));
	float3 upVector 	= normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1] ));
	float3 viewVector	= normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2] ));
	
	#if defined( ALIGNED_TO_UP_AXIS )
		upVector 			= float3(0,1,0);
	#endif

	position 			= (IN.Position.x*rightVector+IN.Position.z*upVector) * g_Size;

#endif
	
	TT1_VERTEX_PS OUT=(TT1_VERTEX_PS)0;
	OUT.Position		= mul( float4( position.xyz, 1), g_WorldViewProj);
	OUT.WorldPosition	= OUT.Position;
	
	// Get the correct UV's for the billboard
	float u = IN.UV.x;
	if( g_FlipUVHorizontal ) u = 1.0 - u;
	float v = IN.UV.y;
	if( g_FlipUVVertical ) v = 1.0 - v;
	OUT.UV				= float2( u, v );
	return OUT;
}
float4 mainBillboardPS(TT1_VERTEX_PS IN) : COLOR
{
	float4 l_Color=tex2D(S0PointSampler, IN.UV);
	return float4(l_Color.xyz*l_Color.a, l_Color.a*g_Alpha);//float4(1, 0, 0, 1);
}

technique TECHNIQUE_NAME
{
	pass p0
	{
#if defined(ONEONEADD)
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
#endif
		VertexShader = compile vs_3_0 mainBillboardVS();
		PixelShader = compile ps_3_0 mainBillboardPS();
	}
}
