#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TGEOMETRY_PS mainVS(TGEOMETRY IN)
{
	TGEOMETRY_PS OUT=(TGEOMETRY_PS)0;
	OUT.HPosition		= mul( float4( IN.Position, 1), g_WorldViewProj);
	OUT.WorldPosition	= OUT.HPosition;
    return OUT;
}

//Dest.rgb*One + Source.rgb*One

float4 mainPS(TGEOMETRY_PS IN) : COLOR
{
	return float4(0.0, 0, 0.5, 0.5);
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}