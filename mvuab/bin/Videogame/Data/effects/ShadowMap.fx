#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

//-----------------------------------------------------------------------------
// Vertex Shader: VertShadow
// Desc: Process vertex for the shadow map
//-----------------------------------------------------------------------------
void VertShadow( float4 Pos : POSITION,
                 float3 Normal : NORMAL,
				 float2 UV : TEXCOORD0,
                 out float4 oPos : POSITION,
                 out float2 Depth : TEXCOORD0, 
				out float2 oUV : TEXCOORD1 )
{
    //
    // Compute the projected coordinates
    //
    oPos = mul( Pos, g_WorldViewProj );

    //
    // Store z and w in our spare texcoord
    //
    Depth.xy = oPos.zw;
	oUV=UV;
}

//-----------------------------------------------------------------------------
// Pixel Shader: PixShadow
// Desc: Process pixel for the shadow map
//-----------------------------------------------------------------------------
void PixShadow( float2 Depth : TEXCOORD0,
				float2 UV : TEXCOORD1,
                out float4 Color : COLOR )
{
    //
    // Depth is z / w
    //
    Color = Depth.x / Depth.y;
	//Color=tex2D(S0LinearSampler, UV);
}

technique ShadowMapTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}