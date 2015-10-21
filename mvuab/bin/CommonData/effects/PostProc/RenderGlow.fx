#include "../vertex_types.fxh"
#include "../samplers.fxh"
#include "../globals.fxh"

UBER_VERTEX_PS mainVS(UBER_VERTEX_VS IN)
{
	UBER_VERTEX_PS OUT=(UBER_VERTEX_PS)0;
    
#if defined( USE_CAL3D_HW )
	float3 l_Position = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	
	// Modify the x coordinate ( symmetry )
	l_Position.x = -l_Position.x;
	float4 l_WorldPosition = float4(l_Position, 1.0);
	
	OUT.WorldPosition = mul(l_WorldPosition, g_WorldMatrix);	
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProj );
	
	float u = IN.UV.x;
	float v = IN.UV.y;
	v = 1.0 - v;
	OUT.UV = float2( u, v);
	
#else
	OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.WorldPosition=OUT.HPosition;
	OUT.UV=IN.UV;
#endif

#if defined( USE_SELF_ILUM )
	OUT.UV2 = IN.UV2;
#endif

#if defined( USE_DIFFUSE_COLOR )
	OUT.Color = IN.Color;
#endif

    return OUT;
}

float4 mainPS(UBER_VERTEX_PS IN) : COLOR
{
	#if defined( USE_DIFFUSE_COLOR )
	return IN.Color;
#elif defined ( USE_DEBUG )
	return g_DebugColor;
#else
	return tex2D(S0LinearSampler,IN.UV);
#endif
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
