#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float min;
float max;

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 base = tex2D(S0PointSampler, UV);
	float dist = distance(IN, float2(0.5,0.5)) * 0.7 ;    
	base.rgb *= smoothstep(min, max, dist);    
    return base; 
}

technique Vignetting0Technique {
	pass p0 {
		PixelShader = compile ps_3_0 mainPS();
	}
}