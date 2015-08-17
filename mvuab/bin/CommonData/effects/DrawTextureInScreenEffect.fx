
#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 lColor = g_DebugColor * tex2D(S0LinearSampler, UV);
    return float4( lColor.xyz * g_Alpha, g_Alpha );
}

technique DrawTextureInScreenTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 mainPS();
    }
}
