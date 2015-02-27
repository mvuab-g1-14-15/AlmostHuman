#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	return tex2D(S0LinearSampler, UV);
}

technique DrawQuadSampler0Technique
{
	pass p0
    {
		PixelShader = compile ps_3_0 mainPS();
	}
}
