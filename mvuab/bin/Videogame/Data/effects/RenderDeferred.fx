#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	//return tex2D(S1PointSampler, UV);
	return float4(UV.x,UV.y,0,1);
}

technique RenderDeferredTechnique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}
