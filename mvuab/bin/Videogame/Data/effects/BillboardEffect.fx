#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TT1_VERTEX_PS mainVS(TT1_VERTEX_VS IN)
{
	TT1_VERTEX_PS OUT=(TT1_VERTEX_PS)0;
    
	OUT.Position		= mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.WorldPosition	= OUT.Position;
	OUT.UV				= IN.UV;
    return OUT;
}

float4 mainPS(TT1_VERTEX_PS IN) : COLOR
{
	return float4(1, 0, 0, 1);
}

technique BillboardTechnique
{
	pass p0
	{	
		CullMode = NONE;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
