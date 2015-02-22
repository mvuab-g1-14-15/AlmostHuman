#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

TNORMAL_T1_VERTEX_PS mainVS(TNORMAL_T1_VERTEX IN)
{
	TNORMAL_T1_VERTEX_PS OUT=(TNORMAL_T1_VERTEX_PS)0;
    OUT.HPosition = mul(float4(IN.Position, 1.0), g_WorldViewProj);
	OUT.WorldPosition = OUT.HPosition;
    OUT.Normal = mul(IN.Normal, (float3x3)g_WorldMatrix);
	OUT.WorldNormal = OUT.Normal;
	OUT.UV = IN.UV;
    return OUT;
}

TMultiRenderTargetPixel mainPS(TNORMAL_T1_VERTEX_PS IN) : COLOR
{
	TMultiRenderTargetPixel OUT=(TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearSampler,IN.UV);
	if(l_DiffuseColor.a<0.5)
		clip(-1);

	OUT.Albedo=float4(l_DiffuseColor.xyz, g_SpecularFactor);
	OUT.Ambient=float4(l_DiffuseColor.xyz*g_AmbientLight, g_SpecularExponent );
	OUT.Normal=float4(Normal2Texture(normalize(IN.WorldNormal)), 1);
	OUT.Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	
	return OUT;
}

technique GenerateGBufferTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
