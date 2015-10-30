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
	
#else
	OUT.HPosition = mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.WorldPosition = mul(float4(IN.Position, 1.0), g_WorldMatrix);
#endif

    return OUT;
}

float4 mainPS(UBER_VERTEX_PS IN) : COLOR
{
    for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i])
        {
            float4 l_LightPos = mul(float4(g_LightsPosition[i], 1.0f), g_WorldMatrix);
            float l_Dst2Vertex = length(IN.WorldPosition - g_CameraPosition);
            float l_Dst2Sun = length(l_LightPos - g_CameraPosition);
            
            if(l_Dst2Vertex > l_Dst2Sun) discard;
        }
    }
    
    return float4(0, 0, 0, 1);
}

technique TECHNIQUE_NAME
{
	pass p0
	{	
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
