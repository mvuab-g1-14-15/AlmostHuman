#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TT1_VERTEX_PS VertexShader(TT1_VERTEX_VS IN)
{
    TT1_VERTEX_PS OUT=(TT1_VERTEX_PS)0;
	
	OUT.Position	= mul( float4( IN.Position, 1), g_WorldViewProj);
	OUT.UV 			= IN.UV;

    return OUT;
}

float4 PixelShader(TT1_VERTEX_PS IN) : COLOR0
{
    // g_TextureWidth width del viewport
    // g_TextureHeight height del vieport
	return g_DebugColor;
}
 
technique TECHNIQUE_NAME
{
    pass Pass1
    {
	    CullMode = None;
        VertexShader = compile vs_3_0 VertexShader();
        PixelShader = compile ps_3_0  PixelShader();
    }
}