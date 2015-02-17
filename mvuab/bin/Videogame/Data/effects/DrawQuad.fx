#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	//return tex2D(S0PointSampler, UV);
	return float4(1,0,0,1);
}

technique DrawQuadSampler0Technique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}
