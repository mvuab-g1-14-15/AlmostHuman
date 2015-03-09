#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

void VertShadow(float4 Pos : POSITION, float3 Normal : NORMAL, out float4 oPos : POSITION, out float2 Depth : TEXCOORD0 )
{
	//
	// Compute the projected coordinates
	//
	oPos = mul( Pos, g_WorldViewProj );
	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
}

float4 PixShadow( float2 Depth : TEXCOORD0) : COLOR
{
	//
	// Depth is z / w
	//
	// 1 – Píxel iluminado
	// 0 – Píxel en sombra
	return Depth.x / Depth.y;
	
}

technique ShadowMapTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}