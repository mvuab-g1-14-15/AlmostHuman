#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TT1_VERTEX_PS mainVS(TT1_VERTEX_VS IN)
{
	TT1_VERTEX_PS OUT=(TT1_VERTEX_PS)0;
    float3 rightVector = normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0] ));
	float3 upVector = normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1] ));
	//rightVector=float3(1,0,0);
	//upVector=float3(0,1,0);
	
	float3 position 	= IN.Position.x*rightVector+IN.Position.z*upVector;
	OUT.Position		= mul( float4( position.xyz, 1), g_WorldViewProj);
	
	OUT.Normal			= cross( rightVector, upVector );
	OUT.WorldPosition	= OUT.Position;
	OUT.WorldTangent	= float4( rightVector.xyz, 1);
	OUT.WorldBinormal	= float4( upVector.xyz, 1);
	
	OUT.UV				= IN.UV;
    return OUT;
}

//Dest.rgb*One + Source.rgb*One

float4 mainPS(TT1_VERTEX_PS IN) : COLOR
{
	float4 l_Color=tex2D(S0PointSampler, IN.UV);
	return float4(l_Color.xyz*l_Color.a, l_Color.a);//float4(1, 0, 0, 1);
}

technique BillboardTechnique
{
	pass p0
	{	
//		CullMode = NONE;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
